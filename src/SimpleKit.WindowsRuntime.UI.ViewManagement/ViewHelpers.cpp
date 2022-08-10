#include "pch.h"
#include "ViewHelpers.h"
#if __has_include("ViewHelpers.g.cpp")
#include "ViewHelpers.g.cpp"
#endif

using namespace winrt::Windows::UI::ViewManagement;

using namespace winrt::Windows::UI::Xaml;

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	void ViewHelpers::OnViewTitleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		auto newVal = e.NewValue();
		if (newVal)
			ApplicationView::GetForCurrentView().Title(newVal.as<hstring>());
	}

	DependencyProperty ViewHelpers::m_ViewTitleProperty
		= DependencyProperty::RegisterAttached
		(
			L"ViewTitle",
			winrt::xaml_typename<hstring>(),
			winrt::xaml_typename<ViewManagement::ViewHelpers>(),
			PropertyMetadata(nullptr, OnViewTitleChanged)
		);
}
