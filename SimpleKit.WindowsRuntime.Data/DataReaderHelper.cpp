#include "pch.h"
#include "DataReaderHelper.h"
#if __has_include("DataReaderHelper.g.cpp")
#include "DataReaderHelper.g.cpp"
#endif

using winrt::Windows::Foundation::IInspectable;
using winrt::Windows::Foundation::PropertyType;

using winrt::Windows::Storage::Streams::DataReader;

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	hstring DataReaderHelper::ReadString(DataReader const& reader)
	{
		int length = reader.ReadUInt32();
		return reader.ReadString(length);
	}

	IInspectable DataReaderHelper::ReadObject(DataReader const& reader)
	{
		auto type = reader.ReadByte();
		switch (type)
		{
		case (int)StreamTypes::NullPtrType:
			return nullptr;
		case (int)StreamTypes::UInt8Type:
			return box_value(reader.ReadByte());
		case (int)StreamTypes::UInt16Type:
			return box_value(reader.ReadUInt16());
		case (int)StreamTypes::UInt32Type:
			return box_value(reader.ReadUInt32());
		case (int)StreamTypes::UInt64Type:
			return box_value(reader.ReadUInt64());
		case (int)StreamTypes::Int16Type:
			return box_value(reader.ReadInt16());
		case (int)StreamTypes::Int32Type:
			return box_value(reader.ReadInt32());
		case (int)StreamTypes::Int64Type:
			return box_value(reader.ReadInt64());
		case (int)StreamTypes::SingleType:
			return box_value(reader.ReadSingle());
		case (int)StreamTypes::DoubleType:
			return box_value(reader.ReadDouble());
		case (int)StreamTypes::BooleanType:
			return box_value(reader.ReadBoolean());
		case (int)StreamTypes::Char16Type:
			return box_value(reader.ReadUInt16());
		case (int)StreamTypes::GuidType:
			return box_value(reader.ReadGuid());
		case (int)StreamTypes::StringType:
			return box_value(ReadString(reader));
		default:
			throw hresult_invalid_argument(L"Unsupported property type.");
		}
	}
}
