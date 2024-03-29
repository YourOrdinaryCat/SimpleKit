#pragma once

#include "TitleBar.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	struct TitleBar : TitleBarT<TitleBar>
	{
	private:
		static void OnIconPropertyChanged
		(
			Windows::UI::Xaml::DependencyObject const& sender,
			Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& args
		);

	public:
		TitleBar();

		void SetTitleBarForCurrentView();
		void OnApplyTemplate() const;

		DEPENDENCY_PROPERTY
		(
			Title,
			Windows::Foundation::IInspectable
		);

		DEPENDENCY_PROPERTY_META
		(
			Icon,
			Windows::UI::Xaml::Controls::IconElement,
			nullptr,
			OnIconPropertyChanged
		);

	private:
		void UpdateIcon() const;

		Windows::ApplicationModel::Core::CoreApplicationViewTitleBar::IsVisibleChanged_revoker m_visibleChangedToken;
		void OnVisibleChanged
		(
			Windows::ApplicationModel::Core::CoreApplicationViewTitleBar const& sender,
			Windows::Foundation::IInspectable const& args
		) const;

		Windows::UI::Xaml::Window::Activated_revoker m_activatedToken;
		void OnActivated
		(
			Windows::Foundation::IInspectable const& sender,
			Windows::UI::Core::WindowActivatedEventArgs const& e
		) const;

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
