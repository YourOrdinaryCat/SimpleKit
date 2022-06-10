#pragma once

// Defines a property for a WinRT class.
#define PROPERTY(type, name)     \
public:                          \
	type name() const            \
	{                            \
		return m_##name##;       \
	}                            \
	void name(type const& value) \
	{                            \
		m_##name## = value;      \
	}                            \
private:                         \
	type m_##name##;

// Defines a get-only property for a WinRT class.
#define GET_PROPERTY(type, name) \
public:                          \
	type name() const            \
	{                            \
		return m_##name##;       \
	}                            \
private:                         \
	void name(type const& value) \
	{                            \
		m_##name## = value;      \
	}                            \
	type m_##name##;

// Defines a dependency property for a WinRT class.
#define DEPENDENCY_PROPERTY(type, name)                                  \
public:                                                                  \
	static winrt::Windows::UI::Xaml::DependencyProperty name##Property() \
	{                                                                    \
		return m_##name##Property;                                       \
	}                                                                    \
	type name() const                                                    \
	{                                                                    \
		return winrt::unbox_value<type>(GetValue(m_##name##Property));   \
	}                                                                    \
	void name(type const& value)                                         \
	{                                                                    \
		SetValue(m_##name##Property, winrt::box_value(value));           \
	}                                                                    \
private:                                                                 \
	static winrt::Windows::UI::Xaml::DependencyProperty m_##name##Property;
