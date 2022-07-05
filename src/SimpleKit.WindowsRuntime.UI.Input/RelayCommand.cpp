#include "pch.h"
#include "RelayCommand.h"
#if __has_include("RelayCommand.g.cpp")
#include "RelayCommand.g.cpp"
#endif

using namespace winrt::SimpleKit::WindowsRuntime::UI::Input;

using namespace winrt::Windows::Foundation;

namespace winrt::SimpleKit::WindowsRuntime::UI::Input::implementation
{
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

	winrt::event_token RelayCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
	{
		return m_canExecuteChanged.add(handler);
	}

	void RelayCommand::CanExecuteChanged(winrt::event_token const& token) noexcept
	{
		m_canExecuteChanged.remove(token);
	}
}
