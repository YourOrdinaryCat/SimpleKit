#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("NavigationHelper.g.cpp")
#include "NavigationHelper.g.cpp"
#endif

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

using namespace winrt::Windows::System;

using namespace winrt::Windows::UI::Core;

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavigationHelper::NavigationHelper(Page const& page) :
		m_page(page),
		m_useNavigationShortcuts(true),
		m_loadedToken(page.Loaded({ this, &NavigationHelper::OnPageLoaded })),
		m_unloadedToken(page.Unloaded({ this, &NavigationHelper::OnPageUnloaded }))
	{
	}

	NavigationHelper::NavigationHelper(Page const& page, bool const& useNavigationShortcuts) :
		m_page(page),
		m_useNavigationShortcuts(useNavigationShortcuts),
		m_loadedToken(page.Loaded({ this, &NavigationHelper::OnPageLoaded })),
		m_unloadedToken(page.Unloaded({ this, &NavigationHelper::OnPageUnloaded }))
	{
	}

	bool NavigationHelper::CanGoBack() const
	{
		if (const auto frame = GetPageFrame())
			return frame.CanGoBack();

		return false;
	}

	void NavigationHelper::GoBack() const
	{
		if (const auto frame = GetPageFrame())
		{
			if (frame.CanGoBack())
				frame.GoBack();
		}
	}

	bool NavigationHelper::CanGoForward() const
	{
		if (const auto frame = GetPageFrame())
			return frame.CanGoForward();

		return false;
	}

	void NavigationHelper::GoForward() const
	{
		if (const auto frame = GetPageFrame())
		{
			if (frame.CanGoForward())
				frame.GoForward();
		}
	}

	IMap<hstring, IInspectable> NavigationHelper::LoadState(NavigationMode const& navigationMode)
	{
		if (const auto frame = GetPageFrame())
		{
			const auto frameState = SessionStateManager::SessionStateForFrame(frame);
			m_PageKey = L"Page-" + to_hstring(frame.BackStackDepth());

			if (navigationMode == NavigationMode::New)
			{
				// Clear existing state for new navigation
				hstring nextPageKey = m_PageKey;
				int nextPageIndex = frame.BackStackDepth();

				while (frameState.HasKey(nextPageKey))
				{
					frameState.Remove(nextPageKey);
					nextPageIndex++;
					nextPageKey = L"Page-" + winrt::to_hstring(nextPageIndex);
				}

				return nullptr;
			}

			// If we were here before, return the preserved state
			if (frameState.HasKey(m_PageKey))
				return frameState.Lookup(m_PageKey).as<IMap<hstring, IInspectable>>();
		}

		return nullptr;
	}

	void NavigationHelper::SaveState(IMap<hstring, IInspectable> const& state) const
	{
		if (const auto frame = GetPageFrame())
		{
			const auto frameState = SessionStateManager::SessionStateForFrame(frame);
			frameState.Insert(m_PageKey, state);
		}
	}

	NavigationHelper::~NavigationHelper()
	{
		m_page = nullptr;
	}

	Frame NavigationHelper::GetPageFrame() const
	{
		const auto page = m_page.get();
		if (page)
			return page.Frame();
		return nullptr;
	}

	void NavigationHelper::OnPageLoaded(IInspectable const&, RoutedEventArgs const&)
	{
		if (m_useNavigationShortcuts && m_page)
		{
			m_backRequestedToken = SystemNavigationManager::GetForCurrentView().
				BackRequested({ this, &NavigationHelper::OnBackRequested });

			// Listen to the window directly so page focus isn't required
			const auto coreWindow = Window::Current().CoreWindow();
			m_acceleratorKeyActivatedToken = coreWindow.Dispatcher().AcceleratorKeyActivated
			(
				{ this, &NavigationHelper::OnAcceleratorKeyActivated }
			);

			m_pointerPressedToken = coreWindow.
				PointerPressed({ this, &NavigationHelper::OnPointerPressed });

			m_navigationShortcutsRegistered = true;
		}
	}

	void NavigationHelper::OnPageUnloaded(IInspectable const&, RoutedEventArgs const&)
	{
		// Once the page unloads, associated tokens should be revoked
		if (m_navigationShortcutsRegistered)
		{
			SystemNavigationManager::GetForCurrentView().
				BackRequested(m_backRequestedToken);

			const auto coreWindow = Window::Current().CoreWindow();
			coreWindow.Dispatcher().
				AcceleratorKeyActivated(m_acceleratorKeyActivatedToken);

			coreWindow.PointerPressed(m_pointerPressedToken);

			m_navigationShortcutsRegistered = false;
		}

		// Remove handler and release the reference to page
		if (const auto page = m_page.get())
		{
			page.Loaded(m_loadedToken);
			page.Unloaded(m_unloadedToken);
		}
	}

	void NavigationHelper::OnBackRequested(IInspectable const&, BackRequestedEventArgs const& args) const
	{
		if (CanGoBack())
		{
			args.Handled(true);
			GoBack();
		}
	}

	void NavigationHelper::OnAcceleratorKeyActivated(CoreDispatcher const&, AcceleratorKeyEventArgs const& args) const
	{
		const auto virtualKey = args.VirtualKey();

		// Only investigate further when Left, Right, or the dedicated Previous or Next keys
		// are pressed
		if ((args.EventType() == CoreAcceleratorKeyEventType::SystemKeyDown ||
			args.EventType() == CoreAcceleratorKeyEventType::KeyDown) &&
			(virtualKey == VirtualKey::Left || virtualKey == VirtualKey::Right ||
				virtualKey == VirtualKey::GoBack || virtualKey == VirtualKey::GoForward))
		{
			const auto coreWindow = Window::Current().CoreWindow();
			const auto downState = CoreVirtualKeyStates::Down;

			const bool menuKey = (coreWindow.GetKeyState(VirtualKey::Menu) & downState) == downState;
			const bool controlKey = (coreWindow.GetKeyState(VirtualKey::Control) & downState) == downState;
			const bool shiftKey = (coreWindow.GetKeyState(VirtualKey::Shift) & downState) == downState;

			const bool noModifiers = !menuKey && !controlKey && !shiftKey;
			const bool onlyAlt = menuKey && !controlKey && !shiftKey;

			if ((virtualKey == VirtualKey::GoBack && noModifiers) ||
				(virtualKey == VirtualKey::Left && onlyAlt))
			{
				// When the previous key or Alt+Left are pressed navigate back
				args.Handled(true);
				GoBack();
			}
			else if ((virtualKey == VirtualKey::GoForward && noModifiers) ||
				(virtualKey == VirtualKey::Right && onlyAlt))
			{
				// When the next key or Alt+Right are pressed navigate forward
				args.Handled(true);
				GoForward();
			}
		}
	}

	void NavigationHelper::OnPointerPressed(CoreWindow const&, PointerEventArgs const& args) const
	{
		const auto properties = args.CurrentPoint().Properties();

		// Ignore button chords with the left, right, and middle buttons
		if (properties.IsLeftButtonPressed() ||
			properties.IsRightButtonPressed() ||
			properties.IsMiddleButtonPressed())
		{
			return;
		}

		// If back or foward are pressed (but not both) navigate appropriately
		const bool backPressed = properties.IsXButton1Pressed();
		const bool forwardPressed = properties.IsXButton2Pressed();

		if (backPressed ^ forwardPressed)
		{
			args.Handled(true);
			if (backPressed)
				GoBack();
			else
				GoForward();
		}
	}
}
