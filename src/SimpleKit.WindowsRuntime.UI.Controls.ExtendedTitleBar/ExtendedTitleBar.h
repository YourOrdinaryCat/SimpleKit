#pragma once

#include "ExtendedTitleBar.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar>
	{
		ExtendedTitleBar();
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::factory_implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar, implementation::ExtendedTitleBar>
	{
	};
}
