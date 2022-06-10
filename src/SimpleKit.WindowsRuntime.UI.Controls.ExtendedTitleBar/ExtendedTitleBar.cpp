#include "pch.h"
#include "ExtendedTitleBar.h"
#if __has_include("ExtendedTitleBar.g.cpp")
#include "ExtendedTitleBar.g.cpp"
#endif

using winrt::Windows::UI::Xaml::DependencyProperty;
using winrt::Windows::UI::Xaml::PropertyMetadata;

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	ExtendedTitleBar::ExtendedTitleBar()
	{
		if (!m_TitleProperty)
			m_registerProperties();

		InitializeComponent();
	}

	void ExtendedTitleBar::m_registerProperties()
	{
		ExtendedTitleBar::m_TitleProperty =
			DependencyProperty::Register(
				L"Title",
				xaml_typename<IInspectable>(),
				xaml_typename<Controls::ExtendedTitleBar>(),
				PropertyMetadata{ nullptr });

		ExtendedTitleBar::m_IconProperty =
			DependencyProperty::Register(
				L"Icon",
				xaml_typename<IInspectable>(),
				xaml_typename<Controls::ExtendedTitleBar>(),
				PropertyMetadata{ nullptr });
	}

	DependencyProperty ExtendedTitleBar::m_TitleProperty{ nullptr };
	DependencyProperty ExtendedTitleBar::m_IconProperty{ nullptr };
}
