#include "pch.h"
#include "DataWriterHelper.h"
#if __has_include("DataWriterHelper.g.cpp")
#include "DataWriterHelper.g.cpp"
#endif

#include "winrt/SimpleKit.WindowsRuntime.Data.h"

using winrt::Windows::Storage::Streams::DataWriter;

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	void DataWriterHelper::WriteString(DataWriter const& writer, hstring const& str)
	{
		writer.WriteByte((uint8_t)StreamTypes::StringType);
		writer.WriteUInt32(writer.MeasureString(str));
		writer.WriteString(str);
	}
}
