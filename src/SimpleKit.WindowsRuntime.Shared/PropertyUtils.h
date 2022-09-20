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

// Defines a static property for a WinRT class.
#define STATIC_PROPERTY(type, name)     \
public:                                 \
	static type name()                  \
	{                                   \
		return m_##name##;              \
	}                                   \
	static void name(type const& value) \
	{                                   \
		m_##name## = value;             \
	}                                   \
private:                                \
	static type m_##name##;

// Defines a static get-only property for a WinRT class.
#define STATIC_GET_PROPERTY(type, name) \
public:                                 \
	static type name()                  \
	{                                   \
		return m_##name##;              \
	}                                   \
private:                                \
	static void name(type const& value) \
	{                                   \
		m_##name## = value;             \
	}                                   \
	static type m_##name##;

// Defines a dependency property for a WinRT class.
#define DEPENDENCY_PROPERTY(name, type, ownerType)                                   \
public:                                                                              \
	static winrt::Windows::UI::Xaml::DependencyProperty name##Property()             \
	{                                                                                \
		return m_##name##Property();                                                 \
	}                                                                                \
	type name() const                                                                \
	{                                                                                \
		return winrt::unbox_value<type>(GetValue(m_##name##Property()));             \
	}                                                                                \
	void name(type const& value)                                                     \
	{                                                                                \
		SetValue(m_##name##Property(), winrt::box_value(value));                     \
	}                                                                                \
private:                                                                             \
	static inline winrt::Windows::UI::Xaml::DependencyProperty& m_##name##Property() \
	{                                                                                \
		static auto m_##name## =                                                     \
			winrt::Windows::UI::Xaml::DependencyProperty::Register                   \
			(                                                                        \
				L#name,                                                              \
				winrt::xaml_typename<##type##>(),                                    \
				winrt::xaml_typename<##ownerType##>(),                               \
				winrt::Windows::UI::Xaml::PropertyMetadata{ nullptr }                \
			);                                                                       \
		return m_##name##;                                                           \
	}

// Defines a dependency property with property metadata for a WinRT class.
#define DEPENDENCY_PROPERTY_META(name, type, ownerType, changeHandler, defaultVal)        \
public:                                                                                   \
	static winrt::Windows::UI::Xaml::DependencyProperty name##Property()                  \
	{                                                                                     \
		return m_##name##Property();                                                      \
	}                                                                                     \
	type name() const                                                                     \
	{                                                                                     \
		return winrt::unbox_value<type>(GetValue(m_##name##Property()));                  \
	}                                                                                     \
	void name(type const& value)                                                          \
	{                                                                                     \
		SetValue(m_##name##Property(), winrt::box_value(value));                          \
	}                                                                                     \
private:                                                                                  \
	static inline winrt::Windows::UI::Xaml::DependencyProperty& m_##name##Property()      \
	{                                                                                     \
		static auto m_##name## =                                                          \
			winrt::Windows::UI::Xaml::DependencyProperty::Register                        \
			(                                                                             \
				L#name,                                                                   \
				winrt::xaml_typename<##type##>(),                                         \
				winrt::xaml_typename<##ownerType##>(),                                    \
				winrt::Windows::UI::Xaml::PropertyMetadata(defaultVal, ##changeHandler##) \
			);                                                                            \
		return m_##name##;                                                                \
	}

// Defines an attached property for a WinRT class.
#define ATTACHED_PROPERTY(name, type, ownerType, targetType)                         \
public:                                                                              \
	static winrt::Windows::UI::Xaml::DependencyProperty name##Property()             \
	{                                                                                \
		return m_##name##Property();                                                 \
	}                                                                                \
	static type Get##name(targetType const& target)                                  \
	{                                                                                \
		return winrt::unbox_value<type>(target.GetValue(m_##name##Property()));      \
	}                                                                                \
	static void Set##name(targetType const& target, type const& value)               \
	{                                                                                \
		target.SetValue(m_##name##Property(), winrt::box_value(value));              \
	}                                                                                \
private:                                                                             \
	static inline winrt::Windows::UI::Xaml::DependencyProperty& m_##name##Property() \
	{                                                                                \
		static auto m_##name## =                                                     \
			winrt::Windows::UI::Xaml::DependencyProperty::RegisterAttached           \
			(                                                                        \
				L#name,                                                              \
				winrt::xaml_typename<##type##>(),                                    \
				winrt::xaml_typename<##ownerType##>(),                               \
				winrt::Windows::UI::Xaml::PropertyMetadata{ nullptr }                \
			);                                                                       \
		return m_##name##;                                                           \
	}

// Defines an attached property with property metadata for a WinRT class.
#define ATTACHED_PROPERTY_META(name, type, ownerType, targetType, changeHandler, defaultVal)  \
public:                                                                                       \
	static winrt::Windows::UI::Xaml::DependencyProperty name##Property()                      \
	{                                                                                         \
		return m_##name##Property();                                                          \
	}                                                                                         \
	static type Get##name(targetType const& target)                                           \
	{                                                                                         \
		return winrt::unbox_value<type>(target.GetValue(m_##name##Property()));               \
	}                                                                                         \
	static void Set##name(targetType const& target, type const& value)                        \
	{                                                                                         \
		target.SetValue(m_##name##Property(), winrt::box_value(value));                       \
	}                                                                                         \
private:                                                                                      \
	static inline winrt::Windows::UI::Xaml::DependencyProperty& m_##name##Property()          \
	{                                                                                         \
		static auto m_##name## =                                                              \
			winrt::Windows::UI::Xaml::DependencyProperty::RegisterAttached                    \
			(                                                                                 \
				L#name,                                                                       \
				winrt::xaml_typename<##type##>(),                                             \
				winrt::xaml_typename<##ownerType##>(),                                        \
				winrt::Windows::UI::Xaml::PropertyMetadata(##defaultVal##, ##changeHandler##) \
			);                                                                                \
		return m_##name##;                                                                    \
	}
