#pragma once

#include "NavigationHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper>
	{
		NavigationHelper(Windows::UI::Xaml::Controls::Page const& page);
		~NavigationHelper();

		bool CanGoBack();
		void GoBack();

		bool CanGoForward();
		void GoForward();

	private:
		weak_ref<Windows::UI::Xaml::Controls::Page> m_page;

		void m_revokeEvents();

		Windows::UI::Core::SystemNavigationManager::BackRequested_revoker m_backRequestedToken;
		Windows::UI::Xaml::Controls::Page::Loaded_revoker m_loadedToken;
		Windows::UI::Xaml::Controls::Page::Unloaded_revoker m_unloadedToken;

		Windows::UI::Core::CoreDispatcher::AcceleratorKeyActivated_revoker m_acceleratorKeyActivatedToken;
		Windows::UI::Core::CoreWindow::PointerPressed_revoker m_pointerPressedToken;

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
