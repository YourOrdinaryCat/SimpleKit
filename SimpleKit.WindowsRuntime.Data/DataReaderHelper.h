#pragma once

#include "DataReaderHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper>
	{
	};
}

namespace winrt::SimpleKit::WindowsRuntime::Data::factory_implementation
{
	struct DataReaderHelper : DataReaderHelperT<DataReaderHelper, implementation::DataReaderHelper>
	{
	};
}
