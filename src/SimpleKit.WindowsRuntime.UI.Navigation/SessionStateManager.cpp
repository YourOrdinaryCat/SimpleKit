#include "pch.h"
#include "SessionStateManager.h"
#if __has_include("SessionStateManager.g.cpp")
#include "SessionStateManager.g.cpp"
#endif

namespace winrt::SimpleKit.WindowsRuntime.UI.Navigation::implementation
{
    int32_t SessionStateManager::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SessionStateManager::MyProperty(int32_t /*value*/)
    {
        throw hresult_not_implemented();
    }
}
