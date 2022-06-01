#pragma once

#include "SessionStateManager.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct SessionStateManager : SessionStateManagerT<SessionStateManager>
	{
		static void RegisterFrame(Windows::UI::Xaml::Controls::Frame const& frame, hstring const& sessionStateKey, hstring const& sessionBaseKey);

		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> SessionStateForFrame(Windows::UI::Xaml::Controls::Frame const& frame);

	private:
		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_sessionState;
		static std::vector<weak_ref<Windows::UI::Xaml::Controls::Frame>> m_registeredFrames;

		static Windows::UI::Xaml::DependencyProperty m_frameSessionStateProperty;
		static Windows::UI::Xaml::DependencyProperty m_frameSessionStateKeyProperty;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct SessionStateManager : SessionStateManagerT<SessionStateManager, implementation::SessionStateManager>
	{
	};
}
