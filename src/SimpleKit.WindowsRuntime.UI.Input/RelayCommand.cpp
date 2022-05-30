#include "pch.h"
#include "RelayCommand.h"
#if __has_include("RelayCommand.g.cpp")
#include "RelayCommand.g.cpp"
#endif

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::implementation
{

#pragma region Using Directives
	using SimpleKit::WindowsRuntime::UI::Input::ExecuteDelegate;
	using SimpleKit::WindowsRuntime::UI::Input::CanExecuteDelegate;

	using Windows::Foundation::EventHandler;
	using Windows::Foundation::IInspectable;
#pragma endregion

	RelayCommand::RelayCommand(ExecuteDelegate const& execute)
	{
		if (execute == nullptr)
			throw hresult_invalid_argument(L"Provided ExecuteDelegate cannot be null.");
		m_execute = execute;
	}

	RelayCommand::RelayCommand(ExecuteDelegate const& execute, CanExecuteDelegate const& canExecute)
		: RelayCommand(execute)
	{
		m_canExecute = canExecute;
	}

	void RelayCommand::Execute(IInspectable const& param)
	{
		m_execute(param);
	}

	bool RelayCommand::CanExecute(IInspectable const& param)
	{
		if (m_canExecute != nullptr) return m_canExecute(param);
		return true;
	}
}
