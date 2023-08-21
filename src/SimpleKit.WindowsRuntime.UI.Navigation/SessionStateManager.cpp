#include "pch.h"
#include "SessionStateManager.h"
#if __has_include("SessionStateManager.g.cpp")
#include "SessionStateManager.g.cpp"
#endif

#include "winrt/SimpleKit.WindowsRuntime.Data.h"

using namespace winrt::SimpleKit::WindowsRuntime::Data;

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::FileProperties;

using namespace winrt::Windows::Storage::Streams;

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;

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
		for (auto&& weakRef : m_registeredFrames)
		{
			auto frame = weakRef.get();
			if (frame)
				SaveFrameNavigationState(frame);
		}

		// Switch to a background context to reduce unnecessary switching
		const winrt::apartment_context context;
		co_await winrt::resume_background();

		// Serialize the session state synchronously to avoid asynchronous access to shared state
		const auto sessionData = InMemoryRandomAccessStream();
		const auto sessionDataWriter = DataWriter(sessionData.GetOutputStreamAt(0));

		DataWriterHelper::WriteObject(sessionDataWriter, m_sessionState);
		co_await sessionDataWriter.StoreAsync();

		const auto stateFolder = ApplicationData::Current().LocalFolder();
		StorageFile file{ co_await stateFolder.CreateFileAsync(m_sessionStateFilename, CreationCollisionOption::ReplaceExisting) };

		const IRandomAccessStream fileStream{ co_await file.OpenAsync(FileAccessMode::ReadWrite) };
		co_await RandomAccessStream::CopyAsync
		(
			sessionData.GetInputStreamAt(0),
			fileStream.GetOutputStreamAt(0)
		);

		// Go back to the original context before returning
		co_await context;
	}

	IAsyncAction SessionStateManager::RestoreAsync()
	{
		return RestoreAsync(hstring{});
	}

	IAsyncAction SessionStateManager::RestoreAsync(hstring const sessionBaseKey)
	{
		m_sessionState.Clear();

		// Switch to a background context to reduce unnecessary switching
		const winrt::apartment_context context;
		co_await winrt::resume_background();

		const auto stateFolder = ApplicationData::Current().LocalFolder();
		const StorageFile stateFile{ co_await stateFolder.GetFileAsync(m_sessionStateFilename) };

		const BasicProperties props{ co_await stateFile.GetBasicPropertiesAsync() };

		// We can't work with anything larger than 4GB
		const auto size = unsigned int(props.Size());
		if (size != props.Size())
			throw hresult_out_of_bounds(L"Session state larger than 4GB");

		const IRandomAccessStreamWithContentType strm{ co_await stateFile.OpenReadAsync() };
		const auto reader = DataReader(strm);

		co_await reader.LoadAsync(size);

		// Deserialize the Session State
		const auto state = DataReaderHelper::ReadObject(reader);
		m_sessionState = state.as<IMap<hstring, IInspectable>>();

		// Go back to the original context before manipulating the frames
		co_await context;

		// Restore any registered frames to their saved state
		for (auto&& weakFrame : m_registeredFrames)
		{
			const auto frame = weakFrame.get();
			if (frame)
			{
				if (!sessionBaseKey.empty())
				{
					const auto key = frame.GetValue(m_frameSessionBaseKeyProperty).try_as<hstring>();
					if (key && key.value() != sessionBaseKey) continue;
				}

				frame.ClearValue(m_frameSessionStateProperty);
				RestoreFrameNavigationState(frame);
			}
		}
	}

	void SessionStateManager::RegisterFrame(Frame const& frame, hstring const& sessionStateKey)
	{
		ThrowIfRegistered(frame);
		RegisterFrameByKey(frame, sessionStateKey);
	}

	void SessionStateManager::RegisterFrame(Frame const& frame, hstring const& sessionStateKey, hstring const& sessionBaseKey)
	{
		ThrowIfRegistered(frame);
		if (!sessionBaseKey.empty())
		{
			frame.SetValue(m_frameSessionBaseKeyProperty, box_value(sessionBaseKey));
			RegisterFrameByKey(frame, winrt::format(L"{}_{}", sessionBaseKey, sessionStateKey));
		}
		else
		{
			RegisterFrameByKey(frame, sessionStateKey);
		}
	}

	void SessionStateManager::ThrowIfRegistered(Frame const& frame) inline
	{
		if (frame.GetValue(m_frameSessionStateKeyProperty))
			throw hresult_invalid_argument(L"Frames can only be registered to one session state key.");

		if (frame.GetValue(m_frameSessionStateProperty))
			throw hresult_illegal_method_call(L"Frames must either be registered before accessing frame session state, or not registered at all.");
	}

	void SessionStateManager::RegisterFrameByKey(Frame const& frame, hstring const& key)
		{
		// Use a dependency property to associate the session key with a frame, and keep a list of frames whose
		// navigation state should be managed
		frame.SetValue(m_frameSessionStateKeyProperty, winrt::box_value(key));
		m_registeredFrames.emplace_back(winrt::make_weak(frame));

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
		std::erase_if
		(
			m_registeredFrames,
			[&frame](weak_ref<Frame> const& e)
			{
				const auto testFrame = e.get();
				return testFrame == nullptr || testFrame == frame;
			}
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

	void SessionStateManager::OnSessionKeyAdded(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		const auto newVal = e.NewValue().as<hstring>();
		const auto wstr = std::wstring_view{ newVal };

		const auto index = wstr.find('_');
		if (index != std::wstring_view::npos)
		{
			const auto stateKey = wstr.substr(index + 1);
			if (!stateKey.empty())
			{
				const auto baseKey = wstr.substr(0, index);
				if (!baseKey.empty())
				{
					RegisterFrame
					(
						d.as<Frame>(),
						winrt::hstring{ stateKey },
						winrt::hstring{ baseKey }
					);
					return;
				}
			}
		}

		RegisterFrame(d.as<Frame>(), newVal);
	}

	void SessionStateManager::RestoreFrameNavigationState(Frame const& frame)
	{
		const auto frameState = SessionStateForFrame(frame);
		if (frameState.HasKey(L"Navigation"))
		{
			const auto navState = frameState.Lookup(L"Navigation");
			frame.SetNavigationState(navState.as<hstring>());
		}
	}

	void SessionStateManager::SaveFrameNavigationState(Frame const& frame)
	{
		const auto frameState = SessionStateForFrame(frame);
		frameState.Insert(L"Navigation", box_value(frame.GetNavigationState()));
	}

	IMap<hstring, IInspectable> SessionStateManager::m_sessionState{ nullptr };
	std::vector<weak_ref<Frame>> SessionStateManager::m_registeredFrames;

	DependencyProperty SessionStateManager::m_frameSessionStateProperty{ nullptr };
	DependencyProperty SessionStateManager::m_frameSessionStateKeyProperty{ nullptr };
	DependencyProperty SessionStateManager::m_frameSessionBaseKeyProperty{ nullptr };
}
