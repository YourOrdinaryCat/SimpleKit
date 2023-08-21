#include "pch.h"
#include "NavPage.h"
#if __has_include("NavPage.g.cpp")
#include "NavPage.g.cpp"
#endif

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavPage::NavPage() :
		m_PageHelper(*this),
		m_pageState(single_threaded_map<hstring, IInspectable>())
	{
	}

	NavPage::NavPage(bool const& useNavigationShortcuts) :
		m_PageHelper(*this, useNavigationShortcuts),
		m_pageState(single_threaded_map<hstring, IInspectable>())
	{
	}

	void NavPage::OnNavigatedTo(NavigationEventArgs const& e)
	{
		if (const auto state = m_PageHelper.LoadState(e.NavigationMode()))
			m_pageState = state;
	}

	void NavPage::OnNavigatedFrom(NavigationEventArgs const&) const
	{
		m_PageHelper.SaveState(m_pageState);
	}

	bool NavPage::TryLoadState(NavigationMode const& navigationMode)
	{
		const auto state = m_PageHelper.LoadState(navigationMode);
		bool available = state != nullptr;

		if (available)
			m_pageState = state;

		return available;
	}

	void NavPage::SaveState() const
	{
		m_PageHelper.SaveState(m_pageState);
	}
}
