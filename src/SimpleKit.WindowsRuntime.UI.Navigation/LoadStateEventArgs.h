#pragma once

#include "LoadStateEventArgs.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct LoadStateEventArgs : LoadStateEventArgsT<LoadStateEventArgs>
	{
		LoadStateEventArgs
		(
			Windows::UI::Xaml::Navigation::NavigationEventArgs const& navigationArgs,
			Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> const& pageState
		);
		
		LoadStateEventArgs
		(
			Windows::UI::Xaml::Navigation::NavigationEventArgs const& navigationArgs
		);

		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> PageState() const
		{
			return m_pageState;
		}

		GET_PROPERTY(Windows::UI::Xaml::Navigation::NavigationEventArgs, NavigationArgs);
		GET_PROPERTY(bool, HasState);

	private:
		Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_pageState;
	};
}
