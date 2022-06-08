#pragma once

#include "NavPage.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavPage : NavPageT<NavPage>
	{
		NavPage();

		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
		void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

		SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper PageHelper()
		{
			return m_pageHelper;
		}

	private:
		SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper m_pageHelper;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
