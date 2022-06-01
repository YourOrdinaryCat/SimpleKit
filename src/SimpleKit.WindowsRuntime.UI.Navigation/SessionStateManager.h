#pragma once

#include "SessionStateManager.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
    struct SessionStateManager : SessionStateManagerT<SessionStateManager>
    {
    };
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
    struct SessionStateManager : SessionStateManagerT<SessionStateManager, implementation::SessionStateManager>
    {
    };
}
