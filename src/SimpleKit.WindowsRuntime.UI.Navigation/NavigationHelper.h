#pragma once

#include "NavigationHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavigationHelper : NavigationHelperT<NavigationHelper>
	{
		NavigationHelper(Windows::UI::Xaml::Controls::Page const& page);

		bool CanGoBack();
		void GoBack();

		bool CanGoForward();
		void GoForward();

		void HandleNavigationToPage(Windows::UI::Xaml::Navigation::NavigationEventArgs const& args);
		void HandleNavigatedFromPage(Windows::UI::Xaml::Navigation::NavigationEventArgs const& args);

		void SaveState(Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const& state);

		winrt::event_token StateRestorationStarted(Windows::Foundation::EventHandler<SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs> const& handler)
		{
			return m_stateRestorationStartedEvent.add(handler);
		}

		void StateRestorationStarted(winrt::event_token const& token) noexcept
		{
			m_stateRestorationStartedEvent.remove(token);
		}

		winrt::event_token PageStateRequested(Windows::Foundation::EventHandler<Windows::UI::Xaml::Navigation::NavigationEventArgs> const& handler)
		{
			return m_pageStateRequestedEvent.add(handler);
		}

		void PageStateRequested(winrt::event_token const& token) noexcept
		{
			m_pageStateRequestedEvent.remove(token);
		}

	private:
		~NavigationHelper();

		weak_ref<Windows::UI::Xaml::Controls::Page> m_page;
		hstring m_pageKey;

		winrt::event<Windows::Foundation::EventHandler<SimpleKit::WindowsRuntime::UI::Navigation::LoadStateEventArgs>> m_stateRestorationStartedEvent;
		winrt::event<Windows::Foundation::EventHandler<Windows::UI::Xaml::Navigation::NavigationEventArgs>> m_pageStateRequestedEvent;

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
