#include "pch.h"
#include "SessionStateManager.h"
#if __has_include("SessionStateManager.g.cpp")
#include "SessionStateManager.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Navigation::implementation
{

#pragma region Using Directives
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

	std::vector<weak_ref<Frame>> SessionStateManager::m_registeredFrames;

	DependencyProperty SessionStateManager::m_frameSessionStateProperty{ nullptr };
	DependencyProperty SessionStateManager::m_frameSessionStateKeyProperty{ nullptr };
}
