#pragma once

#include "DataReaderHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper>
	{
		static hstring ReadString(Windows::Storage::Streams::DataReader const& reader);
		static Windows::Foundation::IInspectable ReadObject(Windows::Storage::Streams::DataReader const& reader);

		static Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> ReadStringToObjectMap(Windows::Storage::Streams::DataReader const& reader);
		static Windows::Foundation::Collections::IMap<Windows::Foundation::IInspectable, Windows::Foundation::IInspectable> ReadMap(Windows::Storage::Streams::DataReader const& reader);
		static Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> ReadVector(Windows::Storage::Streams::DataReader const& reader);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::Data::factory_implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper, implementation::DataReaderHelper>
	{
	};
}
