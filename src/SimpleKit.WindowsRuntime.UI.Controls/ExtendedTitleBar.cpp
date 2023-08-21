#include "pch.h"
#include "ExtendedTitleBar.h"
#if __has_include("ExtendedTitleBar.g.cpp")
#include "ExtendedTitleBar.g.cpp"
#endif

using namespace winrt::Windows::ApplicationModel::Core;

using namespace winrt::Windows::Foundation;

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

using namespace winrt::Windows::UI::Xaml;

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	ExtendedTitleBar::ExtendedTitleBar()
	{
		DefaultStyleKey(winrt::box_value(winrt::name_of<class_type>()));

		m_unloadedToken = Unloaded
		(
			{ this, &ExtendedTitleBar::OnUnloaded }
		);
	}

	void ExtendedTitleBar::SetTitleBarForCurrentView()
	{
		const auto view = ApplicationView::GetForCurrentView();
		const auto coreView = CoreApplication::GetCurrentView();
		const auto window = Window::Current();

		const auto titleBar = view.TitleBar();
		const auto coreTitleBar = coreView.TitleBar();

		const auto buttonBg = Colors::Transparent();
		titleBar.ButtonBackgroundColor(buttonBg);
		titleBar.ButtonInactiveBackgroundColor(buttonBg);

		coreTitleBar.ExtendViewIntoTitleBar(true);
		window.SetTitleBar(*this);

		m_visibleChangedToken = coreTitleBar.IsVisibleChanged
		(
			winrt::auto_revoke,
			{ this, &ExtendedTitleBar::OnVisibleChanged }
		);

		m_activatedToken = window.Activated
		(
			winrt::auto_revoke,
			{ this, &ExtendedTitleBar::OnActivated }
		);
	}

	void ExtendedTitleBar::OnVisibleChanged(CoreApplicationViewTitleBar const& sender, IInspectable const&) const
	{
		if (sender.IsVisible())
			VisualStateManager::GoToState(*this, L"TitleBarVisibleState", true);
		else
			VisualStateManager::GoToState(*this, L"TitleBarCollapsedState", true);
	}

	void ExtendedTitleBar::OnActivated(IInspectable const&, WindowActivatedEventArgs const& e) const
	{
		if (e.WindowActivationState() == CoreWindowActivationState::Deactivated)
			VisualStateManager::GoToState(*this, L"WindowInactiveState", true);
		else
			VisualStateManager::GoToState(*this, L"WindowActiveState", true);
	}

	void ExtendedTitleBar::OnUnloaded(IInspectable const&, RoutedEventArgs const&)
	{
		m_visibleChangedToken.revoke();
		m_activatedToken.revoke();

		Unloaded(m_unloadedToken);
	}
}
