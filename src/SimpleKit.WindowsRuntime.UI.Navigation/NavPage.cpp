#include "pch.h"
#include "NavPage.h"
#if __has_include("NavPage.g.cpp")
#include "NavPage.g.cpp"
#endif

using winrt::SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper;
using winrt::SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs;
using winrt::SimpleKit::WindowsRuntime::UI::Navigation::SessionStateManager;

using winrt::Windows::Foundation::Collections::IMap;

using winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs;
using winrt::Windows::UI::Xaml::Navigation::NavigationMode;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavPage::NavPage() : m_navigationHelper(*this)
	{
	}

	void NavPage::OnNavigatedTo(NavigationEventArgs const& e)
	{
		auto strong_this{ get_strong() };

		auto frameState = SessionStateManager::SessionStateForFrame(Frame());
		m_pageKey = L"Page-" + to_hstring(Frame().BackStackDepth());

		if (e.NavigationMode() == NavigationMode::New ||
			e.NavigationMode() == NavigationMode::Refresh)
		{
			// Clear existing state for new navigation
			hstring nextPageKey = m_pageKey;
			int nextPageIndex = Frame().BackStackDepth();

			while (frameState.HasKey(nextPageKey))
			{
				frameState.Remove(nextPageKey);
				nextPageIndex++;
				nextPageKey = L"Page-" + nextPageIndex;
			}

			this->OnStateLoadRequested(LoadStateEventArgs(e.Parameter(), nullptr));
		}
		else
		{
			// If we were here before, pass the navigation parameter and
			// preserved state
			auto state = frameState.Lookup(m_pageKey).try_as<IMap<hstring, IInspectable>>();
			this->OnStateLoadRequested(LoadStateEventArgs(e.Parameter(), state));
		}
	}

	void NavPage::OnNavigatedFrom(NavigationEventArgs const& e)
	{
		auto strong_this{ get_strong() };

		auto frameState = SessionStateManager::SessionStateForFrame(Frame());
		auto pageState = single_threaded_map<hstring, IInspectable>();

		this->OnStateSaveRequested(pageState);
		frameState.Insert(m_pageKey, pageState);
	}
}
