#include "pch.h"
#include "LoadStateEventArgs.h"
#if __has_include("LoadStateEventArgs.g.cpp")
#include "LoadStateEventArgs.g.cpp"
#endif

using winrt::Windows::Foundation::IInspectable;

using winrt::Windows::Foundation::Collections::IMap;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	LoadStateEventArgs::LoadStateEventArgs(IInspectable navigationParameter, IMap<hstring, IInspectable> pageState)
		: m_navigationParameter(navigationParameter), m_pageState(pageState)
	{
	}
}
