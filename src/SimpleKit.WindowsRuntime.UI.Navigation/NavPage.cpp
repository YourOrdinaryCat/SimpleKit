#include "pch.h"
#include "NavPage.h"
#if __has_include("NavPage.g.cpp")
#include "NavPage.g.cpp"
#endif

using winrt::SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs;
using winrt::SimpleKit::WindowsRuntime::UI::Navigation::SessionStateManager;

using winrt::Windows::Foundation::Collections::IMap;

using winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	void NavPage::OnNavigatedTo(NavigationEventArgs const& e)
	{
		StateHandler().HandleNavigationToPage(e);
	}

	void NavPage::OnNavigatedFrom(NavigationEventArgs const&)
	{
		StateHandler().HandleNavigatedFromPage();
	}
}
