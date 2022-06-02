#pragma once

#include "DataReaderHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper>
	{
		static hstring ReadString(Windows::Storage::Streams::DataReader const& reader);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::Data::factory_implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper, implementation::DataReaderHelper>
	{
	};
}
