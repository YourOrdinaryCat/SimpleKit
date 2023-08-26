#include "pch.h"
#include "MainPage.h"
#include "Views/MainPage.g.cpp"

namespace wux = winrt::Windows::UI::Xaml;

namespace winrt::$cpprootnamespace$::Views::implementation {
	int32_t MainPage::MyProperty() const {
		throw hresult_not_implemented();
	}

	void MainPage::MyProperty(int32_t /* value */) {
		throw hresult_not_implemented();
	}

	void MainPage::ClickHandler(const IInspectable&, const wux::RoutedEventArgs&) {
		MyButton().Content(winrt::box_value(L"Clicked"));
	}
}
