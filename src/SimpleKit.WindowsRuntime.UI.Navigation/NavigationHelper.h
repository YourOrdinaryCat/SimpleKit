#pragma once

#include "NavigationHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper>
	{
		NavigationHelper(Windows::UI::Xaml::Controls::Page const& page);
		~NavigationHelper();

	private:
		Windows::UI::Xaml::Controls::Page m_page;

		void m_revokeEvents();

		Windows::UI::Xaml::Controls::Page::Loaded_revoker m_loadedToken;
		Windows::UI::Xaml::Controls::Page::Unloaded_revoker m_unloadedToken;

		void OnPageLoaded
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Xaml::RoutedEventArgs const& args
		);

		void OnPageUnloaded
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Xaml::RoutedEventArgs const& args
		);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper, implementation::NavigationHelper>
	{
	};
}
