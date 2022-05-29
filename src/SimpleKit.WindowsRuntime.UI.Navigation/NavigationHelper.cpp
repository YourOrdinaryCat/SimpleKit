#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("NavigationHelper.g.cpp")
#include "NavigationHelper.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{

#pragma region Using Directives
	using Windows::Foundation::IInspectable;

	using Windows::System::VirtualKey;
	using Windows::System::VirtualKeyModifiers;

	using Windows::UI::Core::AcceleratorKeyEventArgs;
	using Windows::UI::Core::BackRequestedEventArgs;
	using Windows::UI::Core::CoreAcceleratorKeyEventType;
	using Windows::UI::Core::CoreDispatcher;
	using Windows::UI::Core::CoreVirtualKeyStates;
	using Windows::UI::Core::CoreWindow;
	using Windows::UI::Core::PointerEventArgs;
	using Windows::UI::Core::SystemNavigationManager;

	using Windows::UI::Xaml::RoutedEventArgs;
	using Windows::UI::Xaml::Window;

	using Windows::UI::Xaml::Controls::Page;
#pragma endregion

	NavigationHelper::NavigationHelper(Page const& page)
	{
		m_page = make_weak<Page>(page);

		m_loadedToken = page.Loaded
		(
			winrt::auto_revoke,
			{ this, &NavigationHelper::OnPageLoaded }
		);

		m_unloadedToken = page.Unloaded
		(
			winrt::auto_revoke,
			{ this, &NavigationHelper::OnPageUnloaded }
		);
	}

	NavigationHelper::~NavigationHelper()
	{
		m_revokeEvents();
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

	void NavigationHelper::m_revokeEvents()
	{
		m_loadedToken.revoke();
		m_unloadedToken.revoke();

		m_backRequestedToken.revoke();
		m_acceleratorKeyActivatedToken.revoke();
		m_pointerPressedToken.revoke();
	}

	void NavigationHelper::OnPageLoaded(IInspectable const&, RoutedEventArgs const&)
	{
		auto coreWindow = Window::Current().CoreWindow();

		if (m_page)
		{
			m_backRequestedToken = SystemNavigationManager::GetForCurrentView().BackRequested
			(
				winrt::auto_revoke,
				{ this, &NavigationHelper::OnBackRequested }
			);

			// Listen to the window directly so page focus isn't required
			m_acceleratorKeyActivatedToken = coreWindow.Dispatcher().AcceleratorKeyActivated
			(
				winrt::auto_revoke,
				{ this, &NavigationHelper::OnAcceleratorKeyActivated }
			);

			m_pointerPressedToken = coreWindow.PointerPressed
			(
				winrt::auto_revoke,
				{ this, &NavigationHelper::OnPointerPressed }
			);
		}
	}

	void NavigationHelper::OnPageUnloaded(IInspectable const&, RoutedEventArgs const&)
	{
		// Once the page unloads, associated tokens should be revoked
		m_backRequestedToken.revoke();
		m_acceleratorKeyActivatedToken.revoke();
		m_pointerPressedToken.revoke();
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
