#pragma once

#include "NavPage.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct NavPage : NavPageT<NavPage>
	{
		NavPage();
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct NavPage : NavPageT<NavPage, implementation::NavPage>
	{
	};
}
