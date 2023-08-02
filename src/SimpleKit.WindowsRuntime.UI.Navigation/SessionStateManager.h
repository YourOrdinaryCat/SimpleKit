#pragma once

#include "SessionStateManager.g.h"

#include "PropertyUtils.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
	struct SessionStateManager : SessionStateManagerT<SessionStateManager>
	{
	private:
		static void OnSessionKeyAdded(Windows::UI::Xaml::DependencyObject const& d, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

	public:
		SessionStateManager() = default;
		static void Initialize();

		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> SessionState()
		{
			return m_sessionState;
		}

		static Windows::Foundation::IAsyncAction SaveAsync();
		static Windows::Foundation::IAsyncAction RestoreAsync();
		static Windows::Foundation::IAsyncAction RestoreAsync(hstring sessionBaseKey);

		static void RegisterFrame(Windows::UI::Xaml::Controls::Frame const& frame, hstring const& sessionStateKey);
		static void RegisterFrame(Windows::UI::Xaml::Controls::Frame const& frame, hstring sessionStateKey, hstring const& sessionBaseKey);

		static void UnregisterFrame(Windows::UI::Xaml::Controls::Frame const& frame);

		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> SessionStateForFrame(Windows::UI::Xaml::Controls::Frame const& frame);
		ATTACHED_PROPERTY_META
		(
			SessionKey,
			hstring,
			Windows::UI::Xaml::Controls::Frame,
			nullptr,
			OnSessionKeyAdded
		);

	private:
		inline static const hstring m_sessionStateFilename = L"_sessionState.dat";

		static void RestoreFrameNavigationState(Windows::UI::Xaml::Controls::Frame const& frame);
		static void SaveFrameNavigationState(Windows::UI::Xaml::Controls::Frame const& frame);

		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> m_sessionState;
		static std::vector<weak_ref<Windows::UI::Xaml::Controls::Frame>> m_registeredFrames;

		static Windows::UI::Xaml::DependencyProperty m_frameSessionStateProperty;
		static Windows::UI::Xaml::DependencyProperty m_frameSessionStateKeyProperty;
		static Windows::UI::Xaml::DependencyProperty m_frameSessionBaseKeyProperty;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::factory_implementation
{
	struct SessionStateManager : SessionStateManagerT<SessionStateManager, implementation::SessionStateManager>
	{
	};
}
