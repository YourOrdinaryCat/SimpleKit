#pragma once

#include "DataWriterHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	struct DataWriterHelper : DataWriterHelperT<DataWriterHelper>
	{
		static void WriteString(Windows::Storage::Streams::DataWriter const& writer, hstring const& str);

		static void WriteObject(Windows::Storage::Streams::DataWriter const& writer, Windows::Foundation::IInspectable const& obj);
		static void WriteProperty(Windows::Storage::Streams::DataWriter const& writer, Windows::Foundation::IPropertyValue const& propertyValue);

		static void WriteStringToObjectMap(Windows::Storage::Streams::DataWriter const& writer, Windows::Foundation::Collections::IMap<hstring, IInspectable> const& map);
		static void WriteMap(Windows::Storage::Streams::DataWriter const& writer, Windows::Foundation::Collections::IMap<Windows::Foundation::IInspectable, Windows::Foundation::IInspectable> const& map);
		static void WriteVector(Windows::Storage::Streams::DataWriter const& writer, Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> const& vector);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::Data::factory_implementation
{
	struct DataWriterHelper : DataWriterHelperT<DataWriterHelper, implementation::DataWriterHelper>
	{
	};
}
