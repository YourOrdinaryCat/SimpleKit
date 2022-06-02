#pragma once

#include "DataWriterHelper.g.h"

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	struct DataWriterHelper : DataWriterHelperT<DataWriterHelper>
	{
		static void WriteString(Windows::Storage::Streams::DataWriter const& writer, hstring const& str);
	};
}

namespace winrt::SimpleKit::WindowsRuntime::Data::factory_implementation
{
	struct DataWriterHelper : DataWriterHelperT<DataWriterHelper, implementation::DataWriterHelper>
	{
	};
}
