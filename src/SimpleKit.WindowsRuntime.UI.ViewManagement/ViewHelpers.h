#pragma once

#include "ViewHelpers.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers>
	{
		ViewHelpers() = default;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::factory_implementation
{
	struct ViewHelpers : ViewHelpersT<ViewHelpers, implementation::ViewHelpers>
	{
	};
}
