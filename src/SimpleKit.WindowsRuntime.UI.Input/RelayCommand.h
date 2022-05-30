#pragma once

#include "RelayCommand.g.h"

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::implementation
{
	struct RelayCommand : RelayCommandT<RelayCommand>
	{
		RelayCommand
		(
			SimpleKit::WindowsRuntime::UI::Input::ExecuteDelegate const& execute
		);

		RelayCommand
		(
			SimpleKit::WindowsRuntime::UI::Input::ExecuteDelegate const& execute,
			SimpleKit::WindowsRuntime::UI::Input::CanExecuteDelegate const& canExecute
		);

		void Execute(winrt::Windows::Foundation::IInspectable const& param);
		bool CanExecute(winrt::Windows::Foundation::IInspectable const& param);

		winrt::event_token CanExecuteChanged(Windows::Foundation::EventHandler<Windows::Foundation::IInspectable> const& handler);
		void CanExecuteChanged(winrt::event_token const& token) noexcept;

	private:
		winrt::event<Windows::Foundation::EventHandler<Windows::Foundation::IInspectable>> m_canExecuteChanged;

		SimpleKit::WindowsRuntime::UI::Input::ExecuteDelegate m_execute;
		SimpleKit::WindowsRuntime::UI::Input::CanExecuteDelegate m_canExecute;
	};
}

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::factory_implementation
{
	struct RelayCommand : RelayCommandT<RelayCommand, implementation::RelayCommand>
	{
	};
}
