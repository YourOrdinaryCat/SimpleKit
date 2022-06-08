#pragma once

#include "NavPage.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavPage : NavPageT<NavPage>
	{
		NavPage();

		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
		void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

		SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper StateHandler()
		{
			return m_stateHandler;
		}

	private:
		SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper m_stateHandler{ nullptr };
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
