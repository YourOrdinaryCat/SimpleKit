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
	IAsyncOperation<ApplicationView> ViewHelpers::CreateNewViewAsync(TypeName const& sourcePageType)
	{
		ApplicationView newView{ nullptr };

		const auto coreView = CoreApplication::CreateNewView();
		co_await coreView.Dispatcher().RunAsync(CoreDispatcherPriority::Normal,
			[sourcePageType, &newView]
			{
				const auto frame = Frame();
				frame.Navigate(sourcePageType);

				const auto curr = Window::Current();
				curr.Content(frame);
				curr.Activate();

				newView = ApplicationView::GetForCurrentView();
				const auto id = newView.Id();

				m_frames[id] = winrt::make_weak(frame);
				m_tokens[id] = newView.Consolidated(OnViewConsolidated);
			});

		co_return newView;
	}

	IAsyncOperation<bool> ViewHelpers::ShowNewViewAsync(TypeName const& sourcePageType)
	{
		const ApplicationView newView{ co_await CreateNewViewAsync(sourcePageType) };
		co_return co_await ApplicationViewSwitcher::TryShowAsStandaloneAsync(newView.Id());
	}

	void ViewHelpers::OnViewConsolidated(ApplicationView const& sender, ApplicationViewConsolidatedEventArgs const&)
	{
		const auto id = sender.Id();

		// Set the frame's content to null when the view's consolidated,
		// which prevents memory leaks
		if (const auto frame = m_frames[id].get())
			frame.Content(nullptr);

		sender.Consolidated(m_tokens[id]);

		m_frames.erase(id);
		m_tokens.erase(id);
	}

	void ViewHelpers::OnViewTitleChanged(DependencyObject const&, DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto view = ApplicationView::GetForCurrentView())
		{
			const auto newVal = e.NewValue();
			if (newVal)
				view.Title(newVal.as<hstring>());
		}
	}

	std::map<int, winrt::weak_ref<Frame>> ViewHelpers::m_frames = std::map<int, winrt::weak_ref<Frame>>();
	std::map<int, winrt::event_token> ViewHelpers::m_tokens = std::map<int, winrt::event_token>();
}
