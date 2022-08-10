#pragma once

#include "ViewHelpers.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers>
	{
		ViewHelpers() = default;

		ATTACHED_PROPERTY(Windows::UI::Xaml::Controls::Page, hstring, ViewTitle)

	private:
		static void ViewHelpers::OnViewTitleChanged(Windows::UI::Xaml::DependencyObject const& d, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::factory_implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers, implementation::ViewHelpers>
	{
	};
}
