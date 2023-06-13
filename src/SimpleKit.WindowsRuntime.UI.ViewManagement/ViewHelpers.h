#pragma once

#include "ViewHelpers.g.h"

#include <map>

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers>
	{
		ViewHelpers() = default;

		static Windows::Foundation::IAsyncOperation<Windows::UI::ViewManagement::ApplicationView>
			CreateNewViewAsync(Windows::UI::Xaml::Interop::TypeName const& sourcePageType);

		static Windows::Foundation::IAsyncOperation<bool>
			ShowNewViewAsync(Windows::UI::Xaml::Interop::TypeName const& sourcePageType);

		ATTACHED_PROPERTY_META
		(
			ViewTitle,
			hstring,
			ViewManagement::ViewHelpers,
			Windows::UI::Xaml::Controls::Page,
			OnViewTitleChanged,
			nullptr
		)

	private:
		static std::map<int, winrt::weak_ref<Windows::UI::Xaml::Controls::Frame>> m_frames;
		static std::map<int, winrt::event_token> m_tokens;

		static void OnViewConsolidated(Windows::UI::ViewManagement::ApplicationView const& sender, Windows::UI::ViewManagement::ApplicationViewConsolidatedEventArgs const& args);
		static void OnViewTitleChanged(Windows::UI::Xaml::DependencyObject const& d, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::factory_implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers, implementation::ViewHelpers>
	{
	};
}
