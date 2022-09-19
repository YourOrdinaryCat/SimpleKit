#include "pch.h"
#include "ViewHelpers.h"
#if __has_include("ViewHelpers.g.cpp")
#include "ViewHelpers.g.cpp"
#endif

using namespace winrt::Windows::ApplicationModel::Core;

using namespace winrt::Windows::Foundation;

using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::SimpleKit::WindowsRuntime::UI::ViewManagement::implementation
{
	IAsyncOperation<ApplicationView> ViewHelpers::CreateNewViewAsync(TypeName const sourcePageType)
	{
		auto coreView = CoreApplication::CreateNewView();

		// Temporary switch to the new view's context to be able
		// to use the GetForCurrentView style methods
		winrt::apartment_context context;
		co_await coreView.Dispatcher();

		auto frame = Frame();
		frame.Navigate(sourcePageType);

		auto curr = Window::Current();
		curr.Content(frame);
		curr.Activate();

		auto newView = ApplicationView::GetForCurrentView();

		m_tokens[newView.Id()] = newView.Consolidated(OnViewConsolidated);
		m_frames[newView.Id()] = winrt::make_weak(frame);

		co_await context;
		co_return newView;
	}

	IAsyncOperation<bool> ViewHelpers::ShowNewViewAsync(TypeName const sourcePageType)
	{
		ApplicationView newView{ co_await CreateNewViewAsync(sourcePageType) };
		co_return co_await ApplicationViewSwitcher::TryShowAsStandaloneAsync(newView.Id());
	}

	void ViewHelpers::OnViewConsolidated(ApplicationView const& sender, ApplicationViewConsolidatedEventArgs const&)
	{
		auto id = sender.Id();
		auto frame = m_frames[id].get();

		// Set the frame's content to null when the view's consolidated,
		// which prevents memory leaks
		if (frame)
			frame.Content(nullptr);
		sender.Consolidated(m_tokens[id]);

		m_frames.erase(id);
		m_tokens.erase(id);
	}

	void ViewHelpers::OnViewTitleChanged(DependencyObject const&, DependencyPropertyChangedEventArgs const& e)
	{
		auto newVal = e.NewValue();
		if (newVal)
			ApplicationView::GetForCurrentView().Title(newVal.as<hstring>());
	}

	std::map<int, winrt::weak_ref<Frame>> ViewHelpers::m_frames = std::map<int, winrt::weak_ref<Frame>>();
	std::map<int, winrt::event_token> ViewHelpers::m_tokens = std::map<int, winrt::event_token>();
}
