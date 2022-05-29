#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("NavigationHelper.g.cpp")
#include "NavigationHelper.g.cpp"
#endif

using winrt::Windows::UI::Xaml::Controls::Page;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
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

	void NavigationHelper::m_revokeEvents()
	{
		m_loadedToken.revoke();
		m_unloadedToken.revoke();

		m_acceleratorKeyActivatedToken.revoke();
		m_pointerPressedToken.revoke();
	}

	void NavigationHelper::OnPageLoaded(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
	{
		auto coreWindow = Windows::UI::Xaml::Window::Current().CoreWindow();

		// Listen to the window directly so focus isn't required
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

	void NavigationHelper::OnPageUnloaded(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
	{
		// Once the page unloads, associated tokens should be revoked
		m_acceleratorKeyActivatedToken.revoke();
		m_pointerPressedToken.revoke();
	}

	void NavigationHelper::OnAcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher const& sender, Windows::UI::Core::AcceleratorKeyEventArgs const& args)
	{
	}

	void NavigationHelper::OnPointerPressed(Windows::UI::Core::CoreWindow const& sender, Windows::UI::Core::PointerEventArgs const& args)
	{
	}
}
