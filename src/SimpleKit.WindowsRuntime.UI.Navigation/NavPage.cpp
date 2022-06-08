#include "pch.h"
#include "NavPage.h"
#if __has_include("NavPage.g.cpp")
#include "NavPage.g.cpp"
#endif

using winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavPage::NavPage() : m_pageHelper(*this)
	{
	}

	void NavPage::OnNavigatedTo(NavigationEventArgs const& e)
	{
		m_pageHelper.HandleNavigationToPage(e);
	}

	void NavPage::OnNavigatedFrom(NavigationEventArgs const& e)
	{
		m_pageHelper.HandleNavigatedFromPage(e);
	}
}
