#include "pch.h"
#include "DataReaderHelper.h"
#if __has_include("DataReaderHelper.g.cpp")
#include "DataReaderHelper.g.cpp"
#endif

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

using namespace winrt::Windows::Storage::Streams;

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	hstring DataReaderHelper::ReadString(DataReader const& reader)
	{
		const int length = reader.ReadUInt32();
		return reader.ReadString(length);
	}

	IInspectable DataReaderHelper::ReadObject(DataReader const& reader)
	{
		const auto type = reader.ReadByte();
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
		case (int)StreamTypes::MapStartMarker:
			return ReadMap(reader);
		case (int)StreamTypes::StrToObjMapStartMarker:
			return ReadStringToObjectMap(reader);
		case (int)StreamTypes::VectorStartMarker:
			return ReadVector(reader);
		default:
			throw hresult_invalid_argument(L"Unsupported property type.");
		}
	}

	IMap<hstring, IInspectable> DataReaderHelper::ReadStringToObjectMap(DataReader const& reader)
	{
		const auto map = single_threaded_map<hstring, IInspectable>();
		const auto size = reader.ReadUInt32();

		for (uint32_t index = 0; index < size; index++)
		{
			const auto key = ReadObject(reader).try_as<hstring>();
			const auto value = ReadObject(reader);

			if (key)
				map.Insert(key.value(), value);
			else
				throw hresult_invalid_argument(winrt::format(L"Invalid key at index: {}", index));
		}

		if (reader.ReadByte() != (uint8_t)StreamTypes::StrToObjMapEndMarker)
			throw hresult_invalid_argument(L"Invalid stream.");

		return map;
	}

	IMap<IInspectable, IInspectable> DataReaderHelper::ReadMap(DataReader const& reader)
	{
		const auto map = single_threaded_map<IInspectable, IInspectable>();
		const auto size = reader.ReadUInt32();

		for (uint32_t index = 0; index < size; index++)
		{
			const auto key = ReadObject(reader);
			const auto value = ReadObject(reader);

			if (key)
				map.Insert(key, value);
			else
				throw hresult_invalid_argument(winrt::format(L"Invalid key at index: {}", index));
		}

		if (reader.ReadByte() != (uint8_t)StreamTypes::MapEndMarker)
			throw hresult_invalid_argument(L"Invalid stream.");

		return map;
	}

	IVector<IInspectable> DataReaderHelper::ReadVector(DataReader const& reader)
	{
		const auto vector = single_threaded_vector<IInspectable>();
		const auto size = reader.ReadUInt32();

		for (uint32_t index = 0; index < size; index++)
		{
			const auto value = ReadObject(reader);
			if (value)
				vector.Append(value);
			else
				throw hresult_invalid_argument(winrt::format(L"Invalid key at index: {}", index));
		}

		if (reader.ReadByte() != (uint8_t)StreamTypes::VectorEndMarker)
			throw hresult_invalid_argument(L"Invalid stream.");

		return vector;
	}
}
