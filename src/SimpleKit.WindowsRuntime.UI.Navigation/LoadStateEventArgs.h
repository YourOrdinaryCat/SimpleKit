#pragma once

#include "LoadStateEventArgs.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct LoadStateEventArgs : LoadStateEventArgsT<LoadStateEventArgs>
	{
		LoadStateEventArgs
		(
			Windows::UI::Xaml::Navigation::NavigationEventArgs const& navigationArgs,
			Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const& pageState
		) : m_navigationArgs(navigationArgs), m_pageState(pageState) {}

		Windows::UI::Xaml::Navigation::NavigationEventArgs NavigationArgs()
		{
			return m_navigationArgs;
		}

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> PageState()
		{
			return m_pageState;
		}

	private:
		Windows::UI::Xaml::Navigation::NavigationEventArgs m_navigationArgs;
		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_pageState;
	};
}
