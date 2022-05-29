#include "pch.h"
#include "NavigationHelper.h"
#if __has_include("NavigationHelper.g.cpp")
#include "NavigationHelper.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	using Windows::Foundation::IInspectable;

	using Windows::UI::Core::AcceleratorKeyEventArgs;
	using Windows::UI::Core::CoreDispatcher;
	using Windows::UI::Core::CoreWindow;
	using Windows::UI::Core::PointerEventArgs;

	using Windows::UI::Xaml::RoutedEventArgs;

	using Windows::UI::Xaml::Controls::Page;

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

	void NavigationHelper::OnPageLoaded(IInspectable const& sender, RoutedEventArgs const& args)
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

	void NavigationHelper::OnPageUnloaded(IInspectable const& sender, RoutedEventArgs const& args)
	{
		// Once the page unloads, associated tokens should be revoked
		m_acceleratorKeyActivatedToken.revoke();
		m_pointerPressedToken.revoke();
	}

	void NavigationHelper::OnAcceleratorKeyActivated(CoreDispatcher const& sender, AcceleratorKeyEventArgs const& args)
	{
	}

	void NavigationHelper::OnPointerPressed(CoreWindow const& sender, PointerEventArgs const& args)
	{
	}
}
