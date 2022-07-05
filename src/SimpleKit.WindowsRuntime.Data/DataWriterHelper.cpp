#include "pch.h"
#include "DataWriterHelper.h"
#if __has_include("DataWriterHelper.g.cpp")
#include "DataWriterHelper.g.cpp"
#endif

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

using namespace winrt::Windows::Storage::Streams;

namespace winrt::SimpleKit::WindowsRuntime::Data::implementation
{
	void DataWriterHelper::WriteString(DataWriter const& writer, hstring const& str)
	{
		writer.WriteByte((uint8_t)StreamTypes::StringType);
		writer.WriteUInt32(writer.MeasureString(str));
		writer.WriteString(str);
	}

	void DataWriterHelper::WriteObject(DataWriter const& writer, IInspectable const& obj)
	{
		if (!obj)
		{
			writer.WriteByte((uint8_t)StreamTypes::NullPtrType);
			return;
		}

		auto prop = obj.try_as<IPropertyValue>();
		if (prop)
		{
			WriteProperty(writer, prop);
			return;
		}

		auto strMap = obj.try_as<IMap<hstring, IInspectable>>();
		if (strMap)
		{
			WriteStringToObjectMap(writer, strMap);
			return;
		}

		auto map = obj.try_as<IMap<IInspectable, IInspectable>>();
		if (map)
		{
			WriteMap(writer, map);
			return;
		}

		auto vector = obj.try_as<IVector<IInspectable>>();
		if (vector)
		{
			WriteVector(writer, vector);
			return;
		}

		throw hresult_invalid_argument(L"Unsupported property type.");
	}

	void DataWriterHelper::WriteProperty(DataWriter const& writer, IPropertyValue const& propertyValue)
	{
		switch (propertyValue.Type())
		{
		case PropertyType::UInt8:
			writer.WriteByte((uint8_t)StreamTypes::UInt8Type);
			writer.WriteByte(propertyValue.GetUInt8());
			return;
		case PropertyType::UInt16:
			writer.WriteByte((uint8_t)StreamTypes::UInt16Type);
			writer.WriteUInt16(propertyValue.GetUInt16());
			return;
		case PropertyType::UInt32:
			writer.WriteByte((uint8_t)StreamTypes::UInt32Type);
			writer.WriteUInt32(propertyValue.GetUInt32());
			return;
		case PropertyType::UInt64:
			writer.WriteByte((uint8_t)StreamTypes::UInt64Type);
			writer.WriteUInt64(propertyValue.GetUInt64());
			return;
		case PropertyType::Int16:
			writer.WriteByte((uint8_t)StreamTypes::Int16Type);
			writer.WriteUInt16(propertyValue.GetInt16());
			return;
		case PropertyType::Int32:
			writer.WriteByte((uint8_t)StreamTypes::Int32Type);
			writer.WriteUInt32(propertyValue.GetInt32());
			return;
		case PropertyType::Int64:
			writer.WriteByte((uint8_t)StreamTypes::Int64Type);
			writer.WriteUInt64(propertyValue.GetInt64());
			return;
		case PropertyType::Single:
			writer.WriteByte((uint8_t)StreamTypes::SingleType);
			writer.WriteSingle(propertyValue.GetSingle());
			return;
		case PropertyType::Double:
			writer.WriteByte((uint8_t)StreamTypes::DoubleType);
			writer.WriteDouble(propertyValue.GetDouble());
			return;
		case PropertyType::Boolean:
			writer.WriteByte((uint8_t)StreamTypes::BooleanType);
			writer.WriteBoolean(propertyValue.GetBoolean());
			return;
		case PropertyType::Char16:
			writer.WriteByte((uint8_t)StreamTypes::Char16Type);
			writer.WriteUInt16(propertyValue.GetChar16());
			return;
		case PropertyType::Guid:
			writer.WriteByte((uint8_t)StreamTypes::GuidType);
			writer.WriteGuid(propertyValue.GetGuid());
			return;
		case PropertyType::String:
			WriteString(writer, propertyValue.GetString());
			return;
		default:
			throw hresult_invalid_argument(L"Unsupported property type.");
		}
	}

	void DataWriterHelper::WriteStringToObjectMap(DataWriter const& writer, IMap<hstring, IInspectable> const& map)
	{
		writer.WriteByte((uint8_t)StreamTypes::StrToObjMapStartMarker);
		writer.WriteUInt32(map.Size());

		for (auto&& pair : map)
		{
			WriteObject(writer, box_value(pair.Key()));
			WriteObject(writer, pair.Value());
		}

		writer.WriteByte((uint8_t)StreamTypes::StrToObjMapEndMarker);
	}

	void DataWriterHelper::WriteMap(DataWriter const& writer, IMap<IInspectable, IInspectable> const& map)
	{
		writer.WriteByte((uint8_t)StreamTypes::MapStartMarker);
		writer.WriteUInt32(map.Size());

		for (auto&& pair : map)
		{
			WriteObject(writer, pair.Key());
			WriteObject(writer, pair.Value());
		}

		writer.WriteByte((uint8_t)StreamTypes::MapEndMarker);
	}

	void DataWriterHelper::WriteVector(DataWriter const& writer, IVector<IInspectable> const& vector)
	{
		writer.WriteByte((uint8_t)StreamTypes::VectorStartMarker);
		writer.WriteUInt32(vector.Size());

		for (auto&& item : vector)
			WriteObject(writer, item);

		writer.WriteByte((uint8_t)StreamTypes::VectorEndMarker);
	}
}
