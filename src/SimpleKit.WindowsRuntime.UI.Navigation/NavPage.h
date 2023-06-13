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
		void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e) const;

		bool TryLoadState(Windows::UI::Xaml::Navigation::NavigationMode const& navigationMode);
		void SaveState() const;

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> PageState() const
		{
			return m_pageState;
		}

		GET_PROPERTY(SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper, PageHelper);

	private:
		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_pageState;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
