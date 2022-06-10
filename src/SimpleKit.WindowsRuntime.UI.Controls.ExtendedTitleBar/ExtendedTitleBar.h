#pragma once

#include "ExtendedTitleBar.g.h"

#include "../SimpleKit.WindowsRuntime.Shared/PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar>
	{
		ExtendedTitleBar();

		DEPENDENCY_PROPERTY(Windows::Foundation::IInspectable, Title);
		DEPENDENCY_PROPERTY(Windows::Foundation::IInspectable, Icon);

	private:
		void m_registerProperties();
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::factory_implementation
{
	struct ExtendedTitleBar : ExtendedTitleBarT<ExtendedTitleBar, implementation::ExtendedTitleBar>
	{
	};
}
