#include "pch.h"
#include "SessionStateManager.h"
#if __has_include("SessionStateManager.g.cpp")
#include "SessionStateManager.g.cpp"
#endif

#include "winrt/SimpleKit.WindowsRuntime.Data.h"

using winrt::SimpleKit::WindowsRuntime::Data::DataWriterHelper;

using winrt::Windows::Foundation::IInspectable;
using winrt::Windows::Foundation::IAsyncAction;

using winrt::Windows::Foundation::Collections::IMap;

using winrt::Windows::Storage::ApplicationData;
using winrt::Windows::Storage::CreationCollisionOption;
using winrt::Windows::Storage::FileAccessMode;
using winrt::Windows::Storage::StorageFile;

using winrt::Windows::Storage::Streams::DataWriter;
using winrt::Windows::Storage::Streams::InMemoryRandomAccessStream;
using winrt::Windows::Storage::Streams::IRandomAccessStream;
using winrt::Windows::Storage::Streams::RandomAccessStream;

using winrt::Windows::UI::Xaml::PropertyMetadata;
using winrt::Windows::UI::Xaml::DependencyProperty;

using winrt::Windows::UI::Xaml::Controls::Frame;

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{
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

	IAsyncAction SessionStateManager::SaveAsync()
	{
		// Save the navigation state for all registered frames
		for (weak_ref<Frame> weakRef : m_registeredFrames)
		{
			auto frame = weakRef.get();
			if (frame)
			{
				SaveFrameNavigationState(frame);
			}
		}

		// Switch to a background context to reduce unnecessary switching
		winrt::apartment_context context;
		co_await winrt::resume_background();

		// Serialize the session state synchronously to avoid asynchronous access to shared state
		auto sessionData = InMemoryRandomAccessStream();
		auto sessionDataWriter = DataWriter(sessionData.GetOutputStreamAt(0));
		DataWriterHelper::WriteObject(sessionDataWriter, m_sessionState);

		co_await sessionDataWriter.StoreAsync();

		StorageFile file{ co_await ApplicationData::Current().LocalFolder().
			CreateFileAsync(m_sessionStateFilename, CreationCollisionOption::ReplaceExisting) };
		IRandomAccessStream fileStream{ co_await file.OpenAsync(FileAccessMode::ReadWrite) };

		co_await RandomAccessStream::CopyAsync(
			sessionData.GetInputStreamAt(0),
			fileStream.GetOutputStreamAt(0)
		);

		// Go back to the original context before returning
		co_await context;
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
