#pragma once
#ifndef JSON_HELPER_H
#define JSON_HELPER_H

/// <summary>
/// JSON Loader Features:
/// - Load GameObject Data
/// - Load Settings (Graphics/Controls)
/// - Text/String Data
/// - Parse JSON file to string
/// - Update nodes
/// </summary>

#include <variant>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include <directxmath.h>
using namespace rapidjson;

namespace JSON
{
	typedef std::variant<int, std::string, bool, float, double> DataFromFile;

	enum SettingType {
		GeneralType,
		SoundType,
		ControlType,
		GraphicType,
		Invalid
	};

	// Structs to store loaded object data
	struct ModelData
	{
		std::string ObjectName;
		std::string FileName;
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Scale;
		DirectX::XMFLOAT3 Rotation;
	};

	struct SettingData
	{
		std::string Name;
		DataFromFile Setting;
		SettingType Type;
		std::string Text;
		// Get Data: get<type>(array_name[Position_in_Array].Setting);
	};

	struct TextData
	{
		std::string Name;
		std::string Text;
		// Add more data here e.g. position
	};

	enum class CameraType
	{
		Default,
		Static,
		Debug
	};

	struct CameraData
	{
		std::string Name;
		std::string type;
		DirectX::XMFLOAT3 Position;
	};

	enum LightType
	{
		Point,
		Directional,
		Spotlight
	};

	struct LightData
	{
		std::string Name;
	};

	// Load GameObjects get all data
	std::vector<ModelData> LoadGameObjects( const std::string& fileName );

	// Load text data get all data
	std::vector<TextData> LoadTextDataItems( const std::string& fileName );
	std::vector<TextData> LoadTextDataItems( const std::string& fileName, const std::string& Node );
	
	// Load Setting Files get all data
	std::vector<JSON::SettingData> LoadSettings();

	// Load all Nodes
	std::vector<std::string> LoadFileData( const std::string& fileName );
	std::vector<std::string> LoadFileData( const std::string& fileName, const std::string& Node );
	std::vector<std::pair<std::string, std::string>> LoadFileDataAndName( const std::string& fileName );
	std::pair<std::string, std::string> GetData( Value::ConstMemberIterator value );

	DataFromFile GetDataAny( Value::ConstMemberIterator value );

	// Get file
	Document ParseFile( const std::string& file );
	// Store file
	bool StoreFile( const std::string& fileName, const Document& document );

	// Check node is there
	template <typename DataFormat>
	bool CheckDataIsThere( const std::string& objectName, const DataFormat& document )
	{
		return document.HasMember( objectName.c_str() );;
	}
	// Set string data 
	template <typename DataFormat> void AddObject( DataFormat& document, const std::string& a, const std::string& data )
	{
		Document Document;
		document[a.c_str()].SetString( data.c_str(), Document.GetAllocator() );
	}
	// Set int data 
	template <typename DataFormat> void AddObject( DataFormat& document, const std::string& a, int data )
	{
		Document Document;
		document[a.c_str()].SetInt( data );
	}
	// Set double data 
	template <typename DataFormat> void AddObject( DataFormat& document, const std::string& a, double data )
	{
		Document Document;
		document[a.c_str()].SetDouble( data );
	}
	// Set float data 
	template <typename DataFormat> void AddObject( DataFormat& document, const std::string& a, float data )
	{
		Document Document;
		document[a.c_str()].SetFloat( data );
	}
	// Set bool data 
	template <typename DataFormat> void AddObject( DataFormat& document, const std::string& a, bool data )
	{
		Document Document;
		document[a.c_str()].SetBool( data );
	}

	// Update Data: Can update data by checking name
	template <typename DataTypeToSet>
	void UpdateJSONItemEX( const std::string& jsonFile, const std::string& node, const std::string& dataNode, DataTypeToSet data, const std::string& dataName )
	{
		// Load document
		auto autoData = data;
		Document document = ParseFile( jsonFile );
		// Set node
		if ( document.HasMember( node.c_str() ) )
		{
			if ( document[node.c_str()].IsArray() )
			{
				// Load from file
				for ( rapidjson::Value& Object : document[node.c_str()].GetArray() )
				{
					if ( dataNode != "" )
					{
						if ( Object.HasMember( dataNode.c_str() ) )
						{
							if ( dataName != "" )
							{
								if ( Object["Name"].GetString() == dataName )
								{
									// Check type
									AddObject<rapidjson::Value>( Object, dataNode, autoData );
								}
							}
							else
							{
								AddObject<rapidjson::Value>( Object, dataNode, autoData );
							}
						}
					}
				}
			}
			else
			{
				AddObject<Document>( document, node, autoData );
			}
		}
		StoreFile( jsonFile, document );
	}

	// Update nodes
	template<typename DataTypeToSet>
	void UpdateJSONItem( const std::string& jsonFile, const std::string& node, DataTypeToSet data, const std::string& dataNode )
	{
		UpdateJSONItemEX<DataTypeToSet>( jsonFile, node, dataNode, data, "" );
	}
}

#endif