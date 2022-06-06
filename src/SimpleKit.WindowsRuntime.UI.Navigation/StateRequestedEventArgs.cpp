#include "pch.h"
#include "StateRequestedEventArgs.h"
#if __has_include("StateRequestedEventArgs.g.cpp")
#include "StateRequestedEventArgs.g.cpp"
#endif

using winrt::Windows::Foundation::IInspectable;

using winrt::Windows::Foundation::Collections::IMap;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	StateRequestedEventArgs::StateRequestedEventArgs(IMap<hstring, IInspectable> const& pageState)
		: m_pageState(pageState)
	{
	}
}
