﻿#include "pch.h"
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
	using Windows::UI::Xaml::PropertyMetadata;

	using Windows::UI::Xaml::Controls::Frame;
#pragma endregion

	void SessionStateManager::Initialize()
	{
		if (!m_sessionState)
		{
			m_sessionState = single_threaded_map<hstring, IInspectable>();
			m_registeredFrames = std::vector<weak_ref<Frame>>();

			m_frameSessionStateProperty =
				DependencyProperty::RegisterAttached(
					L"m_frameSessionState",
					xaml_typename<IMap<hstring, IInspectable>>(),
					xaml_typename<Navigation::SessionStateManager>(),
					PropertyMetadata{ nullptr });

			m_frameSessionStateKeyProperty =
				DependencyProperty::RegisterAttached(
					L"m_frameSessionStateKey",
					xaml_typename<hstring>(),
					xaml_typename<Navigation::SessionStateManager>(),
					PropertyMetadata{ nullptr });

			m_frameSessionBaseKeyProperty =
				DependencyProperty::RegisterAttached(
					L"m_frameSessionBaseKey",
					xaml_typename<hstring>(),
					xaml_typename<Navigation::SessionStateManager>(),
					PropertyMetadata{ nullptr });
		}
	}

	void SessionStateManager::RegisterFrame(Frame const& frame, hstring const& sessionStateKey)
	{
		RegisterFrame(frame, sessionStateKey, hstring{ });
	}

	void SessionStateManager::RegisterFrame(Frame const& frame, hstring sessionStateKey, hstring const& sessionBaseKey)
	{
		if (frame.GetValue(m_frameSessionStateKeyProperty) != nullptr)
		{
			throw hresult_invalid_argument(L"Frames can only be registered to one session state key.");
		}

		if (frame.GetValue(m_frameSessionStateProperty) != nullptr)
		{
			throw hresult_illegal_method_call(L"Frames must either be registered before accessing frame session state, or not registered at all.");
		}

		if (sessionBaseKey != hstring{ })
		{
			frame.SetValue(m_frameSessionBaseKeyProperty, box_value(sessionBaseKey));
			sessionStateKey = sessionBaseKey + L"_" + sessionStateKey;
		}

		// Use a dependency property to associate the session key with a frame, and keep a list of frames whose
		// navigation state should be managed
		frame.SetValue(m_frameSessionStateKeyProperty, box_value(sessionStateKey));
		m_registeredFrames.insert(m_registeredFrames.begin(), make_weak(frame));

		// Check to see if navigation state can be restored
		RestoreFrameNavigationState(frame);
	}

	void SessionStateManager::UnregisterFrame(Frame const& frame)
	{
		// Remove session state and remove the frame from the list of frames whose navigation
		// state will be saved
		auto key = frame.GetValue(m_frameSessionStateKeyProperty).as<hstring>();
		if (m_sessionState.HasKey(key))
		{
			m_sessionState.Remove(key);
		}

		// Remove weak references that are no longer reachable
		m_registeredFrames.erase
		(
			std::remove_if
			(
				m_registeredFrames.begin(),
				m_registeredFrames.end(),
				[=](weak_ref<Frame> const& e)
				{
					auto testFrame = e.get();
					return testFrame == nullptr || testFrame == frame;
				}
			),
			m_registeredFrames.end()
					);
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
	DependencyProperty SessionStateManager::m_frameSessionBaseKeyProperty{ nullptr };
}
