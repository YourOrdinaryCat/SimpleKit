#pragma once

#include "LoadStateEventArgs.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct LoadStateEventArgs : LoadStateEventArgsT<LoadStateEventArgs>
	{
		LoadStateEventArgs
		(
			Windows::Foundation::IInspectable navigationParameter,
			Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> pageState
		);

		Windows::Foundation::IInspectable NavigationParameter()
		{
			return m_navigationParameter;
		}

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> PageState()
		{
			return m_pageState;
		}

	private:
		Windows::Foundation::IInspectable m_navigationParameter;
		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_pageState;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct LoadStateEventArgs : LoadStateEventArgsT<LoadStateEventArgs, implementation::LoadStateEventArgs>
	{
	};
}
