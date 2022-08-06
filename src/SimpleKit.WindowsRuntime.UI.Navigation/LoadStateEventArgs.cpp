#include "pch.h"
#include "LoadStateEventArgs.h"
#if __has_include("LoadStateEventArgs.g.cpp")
#include "LoadStateEventArgs.g.cpp"
#endif

using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	LoadStateEventArgs::LoadStateEventArgs(NavigationEventArgs const& navigationArgs)
		: m_NavigationArgs(navigationArgs), m_pageState(nullptr), m_HasState(false)
	{
	}

	LoadStateEventArgs::LoadStateEventArgs(NavigationEventArgs const& navigationArgs, IMap<hstring, Windows::Foundation::IInspectable> const& pageState)
		: m_NavigationArgs(navigationArgs), m_pageState(pageState), m_HasState(true)
	{
	}
}
