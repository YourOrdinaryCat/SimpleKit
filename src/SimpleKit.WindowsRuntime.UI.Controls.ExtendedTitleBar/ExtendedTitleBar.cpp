#include "pch.h"
#include "ExtendedTitleBar.h"
#if __has_include("ExtendedTitleBar.g.cpp")
#include "ExtendedTitleBar.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	ExtendedTitleBar::ExtendedTitleBar()
	{
		//DefaultStyleKey(winrt::box_value(this->GetRuntimeClassName()));
	}
}
