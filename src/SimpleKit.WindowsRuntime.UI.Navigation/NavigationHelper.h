#pragma once

#include "NavigationHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper>
	{
		NavigationHelper(Windows::UI::Xaml::Controls::Page const& page);
		NavigationHelper(Windows::UI::Xaml::Controls::Page const& page, bool const& useNavigationShortcuts);

		bool CanGoBack();
		void GoBack();

		bool CanGoForward();
		void GoForward();

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> LoadState(Windows::UI::Xaml::Navigation::NavigationMode const& navigationMode);
		void SaveState(Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const& state);

	private:
		~NavigationHelper();

		bool m_useNavigationShortcuts;

		weak_ref<Windows::UI::Xaml::Controls::Page> m_page;
		hstring m_pageKey;

		winrt::event_token m_backRequestedToken;
		winrt::event_token m_loadedToken;
		winrt::event_token m_unloadedToken;

		winrt::event_token m_acceleratorKeyActivatedToken;
		winrt::event_token m_pointerPressedToken;

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

		bool m_navigationShortcutsRegistered = false;
		void OnBackRequested
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Core::BackRequestedEventArgs const& args
		);

		void OnAcceleratorKeyActivated
		(
			Windows::UI::Core::CoreDispatcher const& sender,
			Windows::UI::Core::AcceleratorKeyEventArgs const& args
		);

		void OnPointerPressed
		(
			Windows::UI::Core::CoreWindow const& sender,
			Windows::UI::Core::PointerEventArgs const& args
		);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper, implementation::NavigationHelper>
	{
	};
}
