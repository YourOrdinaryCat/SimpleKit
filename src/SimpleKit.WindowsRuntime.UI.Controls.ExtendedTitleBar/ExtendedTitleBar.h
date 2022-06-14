#pragma once

#include "ExtendedTitleBar.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar>
	{
		ExtendedTitleBar();
		void SetTitleBarForCurrentView();

	private:
		Windows::ApplicationModel::Core::CoreApplicationViewTitleBar::LayoutMetricsChanged_revoker m_metricsChangedToken;
		void OnLayoutMetricsChanged
		(
			Windows::ApplicationModel::Core::CoreApplicationViewTitleBar const& sender,
			Windows::Foundation::IInspectable const& args
		);

		Windows::ApplicationModel::Core::CoreApplicationViewTitleBar::IsVisibleChanged_revoker m_visibleChangedToken;
		void OnVisibleChanged
		(
			Windows::ApplicationModel::Core::CoreApplicationViewTitleBar const& sender,
			Windows::Foundation::IInspectable const& args
		);

		Windows::UI::Xaml::Window::Activated_revoker m_activatedToken;
		void OnActivated
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Core::WindowActivatedEventArgs const& e
		);

		winrt::event_token m_unloadedToken;
		void OnUnloaded
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Xaml::RoutedEventArgs const& e
		);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::factory_implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar, implementation::ExtendedTitleBar>
	{
	};
}
