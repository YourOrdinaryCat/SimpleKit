#pragma once

#include "TitleBar.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	struct TitleBar : TitleBarT<TitleBar>
	{
		TitleBar();

		void SetTitleBarForCurrentView();
		void OnApplyTemplate() const;

		DEPENDENCY_PROPERTY
		(
			Title,
			Windows::Foundation::IInspectable,
			Controls::TitleBar
		)

		DEPENDENCY_PROPERTY_META
		(
			Icon,
			Windows::UI::Xaml::Controls::IconElement,
			Controls::TitleBar,
			OnIconPropertyChanged,
			nullptr
		)

	private:
		void UpdateIcon() const;
		static void OnIconPropertyChanged
		(
			Windows::UI::Xaml::DependencyObject const& sender,
			Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& args
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
	struct TitleBar : TitleBarT<TitleBar, implementation::TitleBar>
	{
	};
}
