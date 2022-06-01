#include "pch.h"
#include "SessionStateManager.h"
#if __has_include("SessionStateManager.g.cpp")
#include "SessionStateManager.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{

#pragma region Using Directives
	using Windows::Foundation::IInspectable;

	using Windows::Foundation::Collections::IMap;

	using Windows::UI::Xaml::DependencyProperty;
	using Windows::UI::Xaml::Controls::Frame;
#pragma endregion

	void SessionStateManager::RegisterFrame(Frame const& frame, hstring const& sessionStateKey, hstring const& sessionBaseKey)
	{
		if (frame.GetValue(m_frameSessionStateKeyProperty) != nullptr)
		{
			throw hresult_invalid_argument(L"Frames can only be registered to one session state key.");
		}

		if (frame.GetValue(m_frameSessionStateProperty) != nullptr)
		{
			throw hresult_illegal_method_call(L"Frames must either be registered before accessing frame session state, or not registered at all.");
		}

		//TODO: Register the frame
		throw hresult_not_implemented();
	}

	IMap<hstring, IInspectable> SessionStateManager::SessionStateForFrame(Frame const& frame)
	{
		auto frameState = frame.GetValue(m_frameSessionStateProperty).
			try_as<IMap<hstring, IInspectable>>();

		if (!frameState)
		{
			auto frameSessionKey = frame.GetValue(m_frameSessionStateKeyProperty).try_as<hstring>();
			if (frameSessionKey)
			{
				// Registered frames reflect the corresponding session state
				if (!m_sessionState.HasKey(frameSessionKey.value()))
					m_sessionState.Insert
					(
						frameSessionKey.value(),
						single_threaded_map<hstring, IInspectable>()
					);

				frameState = m_sessionState.Lookup(frameSessionKey.value()).
					try_as<IMap<hstring, IInspectable>>();
			}
			else
			{
				// Frames that aren't registered have transient state
				frameState = single_threaded_map<hstring, IInspectable>();
			}

			frame.SetValue(m_frameSessionStateProperty, frameState);
		}

		return frameState.as<IMap<hstring, IInspectable>>();
	}

	void SessionStateManager::RestoreFrameNavigationState(Frame const& frame)
	{
		auto frameState = SessionStateForFrame(frame);
		if (frameState.HasKey(L"Navigation"))
		{
			auto navState = frameState.Lookup(L"Navigation");
			frame.SetNavigationState(navState.as<hstring>());
		}
	}

	void SessionStateManager::SaveFrameNavigationState(Frame const& frame)
	{
		IMap<hstring, IInspectable> frameState = SessionStateForFrame(frame);
		frameState.Insert(L"Navigation", box_value(frame.GetNavigationState()));
	}

	IMap<hstring, IInspectable> SessionStateManager::m_sessionState{ nullptr };
	std::vector<weak_ref<Frame>> SessionStateManager::m_registeredFrames;

	DependencyProperty SessionStateManager::m_frameSessionStateProperty{ nullptr };
	DependencyProperty SessionStateManager::m_frameSessionStateKeyProperty{ nullptr };
}
