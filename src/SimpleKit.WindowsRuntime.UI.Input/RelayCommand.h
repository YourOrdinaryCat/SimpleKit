#pragma once

#include "RelayCommand.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::implementation
{
	struct RelayCommand : RelayCommandT<RelayCommand>
	{
		RelayCommand() = default;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::factory_implementation
{
	struct RelayCommand : RelayCommandT<RelayCommand, implementation::RelayCommand>
	{
	};
}
