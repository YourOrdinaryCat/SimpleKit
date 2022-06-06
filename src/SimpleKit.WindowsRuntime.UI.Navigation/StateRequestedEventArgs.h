#pragma once

#include "StateRequestedEventArgs.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct StateRequestedEventArgs : StateRequestedEventArgsT<StateRequestedEventArgs>
	{
		StateRequestedEventArgs(Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const& pageState);

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> PageState()
		{
			return m_pageState;
		}

	private:
		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_pageState;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct StateRequestedEventArgs : StateRequestedEventArgsT<StateRequestedEventArgs, implementation::StateRequestedEventArgs>
	{
	};
}
