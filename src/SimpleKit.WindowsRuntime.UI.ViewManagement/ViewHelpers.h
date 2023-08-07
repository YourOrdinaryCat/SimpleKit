#pragma once

#include "ViewHelpers.g.h"

#include <map>

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers>
	{
	private:
		static void OnViewTitleChanged(Windows::UI::Xaml::DependencyObject const& d, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

	public:
		ViewHelpers() = default;

		static Windows::Foundation::IAsyncOperation<Windows::UI::ViewManagement::ApplicationView>
			CreateNewViewAsync(Windows::UI::Xaml::Interop::TypeName const& sourcePageType);

		static Windows::Foundation::IAsyncOperation<bool>
			ShowNewViewAsync(Windows::UI::Xaml::Interop::TypeName const& sourcePageType);

		ATTACHED_PROPERTY_META
		(
			ViewTitle,
			hstring,
			Windows::UI::Xaml::Controls::Page,
			nullptr,
			OnViewTitleChanged
		);

	private:
		static std::map<int, winrt::event_token> m_tokens;

		static void OnViewConsolidated(Windows::UI::ViewManagement::ApplicationView const& sender, Windows::UI::ViewManagement::ApplicationViewConsolidatedEventArgs const& args);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::factory_implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers, implementation::ViewHelpers>
	{
	};
}
