#include "pch.h"
#include "TitleBar.h"
#if __has_include("TitleBar.g.cpp")
#include "TitleBar.g.cpp"
#endif

using namespace winrt::Windows::ApplicationModel::Core;

using namespace winrt::Windows::Foundation;

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::ViewManagement;

using namespace winrt::Windows::UI::Xaml;

namespace winrt::SimpleKit::WindowsRuntime::UI::Controls::implementation
{
	TitleBar::TitleBar()
	{
		DefaultStyleKey(winrt::box_value(this->GetRuntimeClassName()));

		m_IconProperty();
		m_TitleProperty();

		m_unloadedToken = Unloaded
		(
			{ this, &TitleBar::OnUnloaded }
		);
	}

	void TitleBar::SetTitleBarForCurrentView()
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
			{ this, &TitleBar::OnVisibleChanged }
		);

		m_activatedToken = window.Activated
		(
			winrt::auto_revoke,
			{ this, &TitleBar::OnActivated }
		);
	}

	void TitleBar::OnApplyTemplate() const
	{
		__super::OnApplyTemplate();
		this->UpdateIcon();
	}

	void TitleBar::OnIconPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const&)
	{
		sender.as<TitleBar>()->UpdateIcon();
	}

	void TitleBar::UpdateIcon() const
	{
		if (this->Icon())
			VisualStateManager::GoToState(*this, L"IconState", true);
		else
			VisualStateManager::GoToState(*this, L"NoIconState", true);
	}

	void TitleBar::OnVisibleChanged(CoreApplicationViewTitleBar const& sender, IInspectable const&) const
	{
		if (sender.IsVisible())
			VisualStateManager::GoToState(*this, L"TitleBarVisibleState", true);
		else
			VisualStateManager::GoToState(*this, L"TitleBarCollapsedState", true);
	}

	void TitleBar::OnActivated(IInspectable const&, WindowActivatedEventArgs const& e) const
	{
		if (e.WindowActivationState() == CoreWindowActivationState::Deactivated)
			VisualStateManager::GoToState(*this, L"WindowInactiveState", true);
		else
			VisualStateManager::GoToState(*this, L"WindowActiveState", true);
	}

	void TitleBar::OnUnloaded(IInspectable const&, RoutedEventArgs const&)
	{
		m_visibleChangedToken.revoke();
		m_activatedToken.revoke();

		Unloaded(m_unloadedToken);
	}
}
