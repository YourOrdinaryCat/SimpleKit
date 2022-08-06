#pragma once

#include "NavPage.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavPage : NavPageT<NavPage>
	{
		NavPage();
		NavPage(bool const& useNavigationShortcuts);

		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
		void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

		GET_PROPERTY(SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper, PageHelper);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
