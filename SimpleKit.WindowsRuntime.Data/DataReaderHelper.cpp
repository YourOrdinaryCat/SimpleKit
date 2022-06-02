#include "pch.h"
#include "DataReaderHelper.h"
#if __has_include("DataReaderHelper.g.cpp")
#include "DataReaderHelper.g.cpp"
#endif

using winrt::Windows::Storage::Streams::DataReader;

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	hstring DataReaderHelper::ReadString(DataReader const& reader)
	{
		int length = reader.ReadUInt32();
		return reader.ReadString(length);
	}
}
