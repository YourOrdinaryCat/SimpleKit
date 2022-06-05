#include "pch.h"
#include "NavPage.h"
#if __has_include("NavPage.g.cpp")
#include "NavPage.g.cpp"
#endif

using winrt::SimpleKit::WindowsRuntime::UI::Navigation::NavigationHelper;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	NavPage::NavPage() : m_navigationHelper(*this)
	{
	}
}
