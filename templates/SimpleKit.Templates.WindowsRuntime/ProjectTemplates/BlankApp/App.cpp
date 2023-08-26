﻿#include "pch.h"
#include "App.h"

#include "Views/MainPage.h"

namespace views = winrt::$cpprootnamespace$::Views::implementation;
namespace wama = winrt::Windows::ApplicationModel::Activation;
namespace wamc = winrt::Windows::ApplicationModel::Core;
namespace wux = winrt::Windows::UI::Xaml;

namespace winrt::$cpprootnamespace$::implementation {
	App::App() {
#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
		UnhandledException([this](const IInspectable&, const wux::UnhandledExceptionEventArgs& e)
			{
				if (IsDebuggerPresent()) {
					const auto errorMessage = e.Message();
					__debugbreak();
				}
			}
		);
#endif
	}

	void App::OnLaunched(const wama::LaunchActivatedEventArgs& args) const {
		const auto window = wux::Window::Current();
		if (!window.Content())
			window.Content(winrt::make<views::MainPage>());

		if (!args.PrelaunchActivated()) {
			window.Activate();
			wamc::CoreApplication::EnablePrelaunch(true);
		}
	}
}
