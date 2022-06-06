#pragma once

#include "NavPage.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavPage : NavPageT<NavPage>
	{
		NavPage();

		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);
		void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e);

		virtual void OnStateLoadRequested(SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs const&) {};
		virtual void OnStateSaveRequested(Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const&) {};

	private:
		SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper m_navigationHelper;

		hstring m_pageKey;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
