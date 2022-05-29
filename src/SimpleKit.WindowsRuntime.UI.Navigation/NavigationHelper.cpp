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
		m_page = page;

		m_loadedToken = m_page.Loaded
		(
			winrt::auto_revoke,
			{ this, &NavigationHelper::OnPageLoaded }
		);

		m_unloadedToken = m_page.Unloaded
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
	}

	void NavigationHelper::OnPageLoaded(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
	{
	}

	void NavigationHelper::OnPageUnloaded(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
	{
	}
}
