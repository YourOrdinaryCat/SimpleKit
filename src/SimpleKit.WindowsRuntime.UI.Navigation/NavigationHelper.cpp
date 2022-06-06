#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("NavigationHelper.g.cpp")
#include "NavigationHelper.g.cpp"
#endif

using winrt::SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs;
using winrt::SimpleKit::WindowsRuntime::UI::Navigation::StateRequestedEventArgs;

using winrt::Windows::Foundation::IInspectable;

using winrt::Windows::Foundation::Collections::IMap;

using winrt::Windows::System::VirtualKey;
using winrt::Windows::System::VirtualKeyModifiers;

using winrt::Windows::UI::Core::AcceleratorKeyEventArgs;
using winrt::Windows::UI::Core::BackRequestedEventArgs;
using winrt::Windows::UI::Core::CoreAcceleratorKeyEventType;
using winrt::Windows::UI::Core::CoreDispatcher;
using winrt::Windows::UI::Core::CoreVirtualKeyStates;
using winrt::Windows::UI::Core::CoreWindow;
using winrt::Windows::UI::Core::PointerEventArgs;
using winrt::Windows::UI::Core::SystemNavigationManager;

using winrt::Windows::UI::Xaml::RoutedEventArgs;
using winrt::Windows::UI::Xaml::Window;

using winrt::Windows::UI::Xaml::Controls::Page;

using winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs;
using winrt::Windows::UI::Xaml::Navigation::NavigationMode;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavigationHelper::NavigationHelper(Page const& page) :
		m_page(page),
		m_loadedToken(page.Loaded({ this, &NavigationHelper::OnPageLoaded })),
		m_unloadedToken(page.Unloaded({ this, &NavigationHelper::OnPageUnloaded }))
	{
	}

	bool NavigationHelper::CanGoBack()
	{
		auto page = m_page.get();
		if (page != nullptr)
		{
			auto frame = page.Frame();
			return (frame != nullptr && frame.CanGoBack());
		}

		return false;
	}

	void NavigationHelper::GoBack()
	{
		auto page = m_page.get();
		if (page != nullptr)
		{
			auto frame = page.Frame();
			if (frame != nullptr && frame.CanGoBack())
				frame.GoBack();
		}
	}

	bool NavigationHelper::CanGoForward()
	{
		auto page = m_page.get();
		if (page != nullptr)
		{
			auto frame = page.Frame();
			return (frame != nullptr && frame.CanGoForward());
		}

		return false;
	}

	void NavigationHelper::GoForward()
	{
		auto page = m_page.get();
		if (page != nullptr)
		{
			auto frame = page.Frame();
			if (frame != nullptr && frame.CanGoForward())
				frame.GoForward();
		}
	}

	void NavigationHelper::HandleNavigationToPage(NavigationEventArgs const& args)
	{
		auto page = m_page.get();
		if (page)
		{
			auto frameState = SessionStateManager::SessionStateForFrame(page.Frame());
			m_pageKey = L"Page-" + to_hstring(page.Frame().BackStackDepth());

			if (args.NavigationMode() == NavigationMode::New)
			{
				// Clear existing state for new navigation
				hstring nextPageKey = m_pageKey;
				int nextPageIndex = page.Frame().BackStackDepth();

				while (frameState.HasKey(nextPageKey))
				{
					frameState.Remove(nextPageKey);
					nextPageIndex++;
					nextPageKey = L"Page-" + nextPageIndex;
				}

				m_stateRestorationStartedEvent(*this, LoadStateEventArgs(args.Parameter(), nullptr));
			}
			else
			{
				// If we were here before, pass the navigation parameter and
				// preserved state
				auto state = frameState.Lookup(m_pageKey).try_as<IMap<hstring, IInspectable>>();
				m_stateRestorationStartedEvent(*this, LoadStateEventArgs(args.Parameter(), state));
			}
		}
	}

	void NavigationHelper::HandleNavigatedFromPage()
	{
		auto page = m_page.get();
		if (page)
		{
			auto frameState = SessionStateManager::SessionStateForFrame(page.Frame());
			auto pageState = single_threaded_map<hstring, IInspectable>();

			m_pageStateRequestedEvent(*this, StateRequestedEventArgs(pageState));
			frameState.Insert(m_pageKey, pageState);
		}
	}

	NavigationHelper::~NavigationHelper()
	{
		m_page = nullptr;
	}

	void NavigationHelper::OnPageLoaded(IInspectable const&, RoutedEventArgs const&)
	{
		auto coreWindow = Window::Current().CoreWindow();
		if (m_page)
		{
			m_backRequestedToken = SystemNavigationManager::GetForCurrentView().
				BackRequested({ this, &NavigationHelper::OnBackRequested });

			// Listen to the window directly so page focus isn't required
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
			auto coreWindow = Window::Current().CoreWindow();
			SystemNavigationManager::GetForCurrentView().
				BackRequested(m_backRequestedToken);

			coreWindow.Dispatcher().
				AcceleratorKeyActivated(m_acceleratorKeyActivatedToken);

			coreWindow.PointerPressed(m_pointerPressedToken);

			m_navigationShortcutsRegistered = false;
		}

		// Remove handler and release the reference to page
		if (auto page = m_page.get())
		{
			page.Loaded(m_loadedToken);
			page.Unloaded(m_unloadedToken);
		}
	}

	void NavigationHelper::OnBackRequested(IInspectable const&, BackRequestedEventArgs const& args)
	{
		if (CanGoBack())
		{
			args.Handled(true);
			GoBack();
		}
	}

	void NavigationHelper::OnAcceleratorKeyActivated(CoreDispatcher const&, AcceleratorKeyEventArgs const& args)
	{
		auto virtualKey = args.VirtualKey();

		// Only investigate further when Left, Right, or the dedicated Previous or Next keys
		// are pressed
		if ((args.EventType() == CoreAcceleratorKeyEventType::SystemKeyDown ||
			args.EventType() == CoreAcceleratorKeyEventType::KeyDown) &&
			(virtualKey == VirtualKey::Left || virtualKey == VirtualKey::Right ||
				virtualKey == VirtualKey::GoBack || virtualKey == VirtualKey::GoForward))
		{
			auto coreWindow = Window::Current().CoreWindow();
			auto downState = CoreVirtualKeyStates::Down;

			bool menuKey = (coreWindow.GetKeyState(VirtualKey::Menu) & downState) == downState;
			bool controlKey = (coreWindow.GetKeyState(VirtualKey::Control) & downState) == downState;
			bool shiftKey = (coreWindow.GetKeyState(VirtualKey::Shift) & downState) == downState;

			bool noModifiers = !menuKey && !controlKey && !shiftKey;
			bool onlyAlt = menuKey && !controlKey && !shiftKey;

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

	void NavigationHelper::OnPointerPressed(CoreWindow const&, PointerEventArgs const& args)
	{
		auto properties = args.CurrentPoint().Properties();

		// Ignore button chords with the left, right, and middle buttons
		if (properties.IsLeftButtonPressed() ||
			properties.IsRightButtonPressed() ||
			properties.IsMiddleButtonPressed())
		{
			return;
		}

		// If back or foward are pressed (but not both) navigate appropriately
		bool backPressed = properties.IsXButton1Pressed();
		bool forwardPressed = properties.IsXButton2Pressed();

		if (backPressed ^ forwardPressed)
		{
			args.Handled(true);
			if (backPressed)
			{
				GoBack();
			}
			else
			{
				GoForward();
			}
		}
	}
}
