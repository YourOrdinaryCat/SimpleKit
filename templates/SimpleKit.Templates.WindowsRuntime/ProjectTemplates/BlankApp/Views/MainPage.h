#pragma once

#include "Views/MainPage.g.h"

namespace winrt::$cpprootnamespace$::Views::implementation {
	/**
	 * @brief Shown to the user when the app is launched normally.
	*/
	struct MainPage : MainPageT<MainPage> {
		MainPage() {
			// Xaml objects should not call InitializeComponent during construction.
			// See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
		}

		int32_t MyProperty() const;
		void MyProperty(int32_t value);

		void ClickHandler(const Windows::Foundation::IInspectable&, const Windows::UI::Xaml::RoutedEventArgs&);
	};
}

namespace winrt::$cpprootnamespace$::Views::factory_implementation {
	struct MainPage : MainPageT<MainPage, implementation::MainPage> {
	};
}
