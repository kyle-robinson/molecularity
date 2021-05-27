#include "stdafx.h"
#include "JSON_Helper.h"

// Parse game object data from file
std::vector<JSON::ModelData> JSON::LoadGameObjects( const std::string& fileName )
{
	Document document;
	ModelData objectData;
	std::vector<ModelData> data;
	
	document = ParseFile( fileName );
	if ( !document.IsNull() )
	{
		//load from file
		for ( Value& GameObject : document["GameObjects"].GetArray() )
		{
			// object name
			if ( CheckDataIsThere<Value>( "Name", GameObject ) )
			{
				objectData.ObjectName = GameObject["Name"].GetString();
			}
			// object file path
			if ( CheckDataIsThere<Value>( "FileName", GameObject ) )
			{
				objectData.FileName = GameObject["FileName"].GetString();
			}
			// object position data
			if ( CheckDataIsThere<Value>( "Position", GameObject ) )
			{
				objectData.Position = {
					GameObject["Position"][0].GetFloat(),
					GameObject["Position"][1].GetFloat(),
					GameObject["Position"][2].GetFloat()
				};
			}
			else if (  CheckDataIsThere<Value>("PosX", GameObject ) &&
				CheckDataIsThere<Value>( "PosY", GameObject ) &&
				CheckDataIsThere<Value>("PosZ", GameObject) )
			{

				objectData.Position.x = GameObject["PosX"][0].GetFloat();
				objectData.Position.y = GameObject["PosY"][0].GetFloat();
				objectData.Position.z = GameObject["PosZ"][0].GetFloat();
			}
			else if ( CheckDataIsThere<Value>( "PositionX", GameObject )
				&& CheckDataIsThere<Value>( "PositionY", GameObject ) &&
				CheckDataIsThere<Value>( "PositionZ", GameObject ) )
			{
				objectData.Position.x = GameObject["PositionX"][0].GetFloat();
				objectData.Position.y = GameObject["PositionY"][0].GetFloat();
				objectData.Position.z = GameObject["PositionZ"][0].GetFloat();
			}
			else
			{
				objectData.Position.x = 0;
				objectData.Position.y = 0;
				objectData.Position.z = 0;
			}
				
			//scale data
			if ( CheckDataIsThere<Value>( "Scale", GameObject ) )
			{
				objectData.Scale = {
					GameObject["Scale"][0].GetFloat(),
					GameObject["Scale"][1].GetFloat(),
					GameObject["Scale"][2].GetFloat()
				};
			}
			else if ( CheckDataIsThere<Value>( "ScaleX", GameObject ) &&
				CheckDataIsThere<Value>( "ScaleY", GameObject ) &&
				CheckDataIsThere<Value>( "ScaleZ", GameObject ) )
			{

				objectData.Scale.x = GameObject["ScaleX"][0].GetFloat();
				objectData.Scale.y = GameObject["ScaleY"][0].GetFloat();
				objectData.Scale.z = GameObject["ScaleZ"][0].GetFloat();
			}
			else
			{
				objectData.Scale.x = 0;
				objectData.Scale.y = 0;
				objectData.Scale.z = 0;
			}

			if ( CheckDataIsThere<Value>( "Rotation", GameObject ) )
			{
				objectData.Rotation = {
					GameObject["Rotation"][0].GetFloat(),
					GameObject["Rotation"][1].GetFloat(),
					GameObject["Rotation"][2].GetFloat()
				};
			}
			else if ( CheckDataIsThere<Value>( "RotX", GameObject ) &&
				CheckDataIsThere<Value>( "RotY", GameObject ) &&
				CheckDataIsThere<Value>( "RotZ", GameObject ) )
			{

				objectData.Rotation.x = GameObject["RotX"][0].GetFloat();
				objectData.Rotation.y = GameObject["RotY"][0].GetFloat();
				objectData.Rotation.z = GameObject["RotZ"][0].GetFloat();
			}
			else if ( CheckDataIsThere<Value>( "RotationX", GameObject ) &&
				CheckDataIsThere<Value>( "RotationY", GameObject ) &&
				CheckDataIsThere<Value>( "RotationZ", GameObject ) )
			{
				objectData.Rotation.x = GameObject["RotationX"][0].GetFloat();
				objectData.Rotation.y = GameObject["RotationY"][0].GetFloat();
				objectData.Rotation.z = GameObject["RotationZ"][0].GetFloat();
			}
			else
			{
				objectData.Rotation.x = 0;
				objectData.Rotation.y = 0;
				objectData.Rotation.z = 0;
			}
			data.push_back( std::move( objectData ) );
		}
	}

	// Check if data was loaded 
	if ( data.size() == 0 )
		ErrorLogger::Log( "Error:: Failed to parse JSON file data!" );
	
	return data;
}
// parse text from file
std::vector<JSON::TextData> JSON::LoadTextDataItems( const std::string& fileName )
{
	std::vector<TextData> data;
	Document document = ParseFile( fileName );

	// load Data in data
	for ( Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object )
	{
		if (Object->value.IsArray()) {
			for (Value& GameObject : document[Object->name.GetString()].GetArray())
			{
				// Store strings in relevant structs and push to vector
				TextData textData;
				if (CheckDataIsThere<Value>("Name", GameObject))
					textData.Name = GameObject["Name"].GetString();
				if (CheckDataIsThere<Value>("Text", GameObject))
					textData.Text = GameObject["Text"].GetString();
				data.push_back(std::move(textData));
			}
		}
	}

	// Check if data was loaded
	if ( data.size() == 0 )
		ErrorLogger::Log( "Error:: No data found when parsing text from JSON file!" );
	
	return data;
}

std::vector<JSON::TextData> JSON::LoadTextDataItems(const std::string& fileName, const std::string& Node)
{
	std::vector<TextData> data;
	Document document = ParseFile(fileName);

	// load Data in data
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object)
	{
		if (Object->name.GetString() == Node) {
			if (Object->value.IsArray()) {
				for (Value& GameObject : document[Object->name.GetString()].GetArray())
				{
					// Store strings in relevant structs and push to vector
					TextData textData;
					if (CheckDataIsThere<Value>("Name", GameObject))
						textData.Name = GameObject["Name"].GetString();
					if (CheckDataIsThere<Value>("Text", GameObject))
						textData.Text = GameObject["Text"].GetString();
					data.push_back(std::move(textData));
				}
			}
		}
	}

	// Check if data was loaded
	if (data.size() == 0)
		ErrorLogger::Log("Error:: No data found when parsing text from JSON file!");

	return data;
}


//load all settings
std::vector<JSON::SettingData> JSON::LoadSettings()
{		
	std::string p;
	SettingType type;
	std::vector< JSON::SettingData> Settings;
	Document document = ParseFile( "Settings.JSON" );

	// Parse data from file
	for ( Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object ) 
	{
		//set setting type
		std::string objectName = Object->name.GetString();
	
		if (objectName == "General") {
			type = SettingType::GeneralType;
		}
		else if (objectName == "Controls") {
			type = SettingType::ControlType;
		}
		else if (objectName == "Sound") {
			type = SettingType::SoundType;
		}
		else if(objectName=="Graphics")
		{
			type = SettingType::GraphicType;
		}
		else
		{
			type = SettingType::Invalid;
		}
		
		//load data
		if ( Object->value.IsArray())
		{
			for ( Value& GameObject : document[Object->name.GetString()].GetArray() )
			{
				for ( Value::ConstMemberIterator itr = GameObject.MemberBegin();
					itr != GameObject.MemberEnd(); ++itr )
				{
					// Store strings in relevant structs and push to vector
					SettingData Setting;
					std::string c = itr->name.GetString();
					Setting.Name = c;
					Setting.Setting = GetDataAny( itr );
					Setting.Type =type;
					
					Settings.push_back( Setting );
				}
			}
		}
		else
		{
			if (type != SettingType::Invalid) {
				SettingData Setting;
				Setting.Name = Object->name.GetString();
				Setting.Setting = GetDataAny(Object);
				Setting.Type = type;
				Settings.push_back(Setting);
			}
		}
	}

	//TODO TextSettings
	
	std::vector<TextData> text= LoadTextDataItems("Text_Eng.json","Settings_Names");
	
	for (UINT i = 0; i < Settings.size(); i++)
	{
		Settings[i].Text = text[i].Text;
	}

	return Settings;
}


//get data as string
std::vector<std::string> JSON::LoadFileData( const std::string& fileName )
{
	std::vector<std::string> data;
	Document document = ParseFile( fileName );
	
	// Parse data into strings
	for ( Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object )
	{
		if( Object->value.IsArray() )
		{
			for ( Value& GameObject : document[Object->name.GetString()].GetArray() )
			{
				for ( Value::ConstMemberIterator itr = GameObject.MemberBegin();
					itr != GameObject.MemberEnd(); ++itr )
				{
					data.push_back( std::move( GetData( itr ).second ) );
				}
			}
		}
		else
		{
			data.push_back( std::move( GetData( Object ).second ) );
		}
	}

	return data;
}
std::vector<std::string> JSON::LoadFileData(const std::string& fileName, const std::string& Node)
{
	std::vector<std::string> data;
	Document document = ParseFile(fileName);

	// Parse data into strings
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object)
	{
		if (Object->name.GetString() == Node) {
			if (Object->value.IsArray())
			{
				for (Value& GameObject : document[Object->name.GetString()].GetArray())
				{
					for (Value::ConstMemberIterator itr = GameObject.MemberBegin();
						itr != GameObject.MemberEnd(); ++itr)
					{
						if (itr->value.IsArray()){
							for (Value& GameObject : document[itr->name.GetString()].GetArray())
							{
								for (Value::ConstMemberIterator itr2 = GameObject.MemberBegin();
									itr2 != GameObject.MemberEnd(); ++itr2)
								{
							
									data.push_back(std::move(GetData(itr2).second));
								}
							}
						}
						else {
							data.push_back(std::move(GetData(itr).second));
						}
					}
				}
			}
			else
			{
				data.push_back(std::move(GetData(Object).second));
			}
		}
	}

	return data;
}
// Parse object data, and unique name as string
std::vector<std::pair<std::string, std::string>> JSON::LoadFileDataAndName( const std::string& fileName )
{
	Document document = ParseFile( fileName );
	std::vector<std::pair<std::string, std::string>> data;
	
	// Parse data into strings
	for ( Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object )
	{
		if ( Object->value.IsArray() )
		{
			for ( Value& GameObject : document[Object->name.GetString()].GetArray() )
			{
				for ( Value::ConstMemberIterator itr = GameObject.MemberBegin();
					itr != GameObject.MemberEnd(); ++itr )
				{
					data.push_back( std::move( GetData( itr ) ) );
				}
			}
		}
		else
		{
			data.push_back( std::move( GetData( Object ) ) );
		}
	}

	return data;
}
// Parse data into strings
std::pair<std::string, std::string> JSON::GetData( Value::ConstMemberIterator itr )
{
	std::string stringData;

	switch ( itr->value.GetType() )
	{
	case( kStringType ):
		stringData = itr->value.GetString();
		break;
	case( kNumberType ):
		if ( itr->value.IsInt() )
			stringData = std::to_string( itr->value.GetInt() );
		else if ( itr->value.IsUint() )
			stringData = std::to_string( itr->value.GetUint() );
		else
			stringData = std::to_string( itr->value.GetFloat() );
		break;
	case( kArrayType ):
		break;
	case( kFalseType ):
		stringData = "False";
		break;
	case( kTrueType ):
		stringData = "True";
		break;
	default:
		break;
	}

	// Store strings in relevant structs and push to vector
	std::string name = itr->name.GetString();
	std::pair<std::string, std::string> data( name, stringData );
	return data;
}

JSON::DataFromFile JSON::GetDataAny( Value::ConstMemberIterator itr )
{
	DataFromFile data;

	switch ( itr->value.GetType() )
	{
	case( kStringType ):
	{
		std::string boo = itr->value.GetString();
		data = boo;
	}
		break;
	case(kNumberType):
		if ( itr->value.IsInt() )
			data = itr->value.GetInt();
		else if ( itr->value.IsDouble() )
			data = itr->value.GetDouble();
		else
			data = itr->value.GetFloat();
		break;
	case( kArrayType ):
		break;
	case( kFalseType ):
	case( kTrueType ):
		data = itr->value.GetBool();
		break;
	default:
		break;
	}

	return data;
}

bool JSON::StoreFile( const std::string& fileName, const Document& document )
{
	// Write back to file
	std::ofstream fileStream("Resources\\JSON\\" + fileName);
	OStreamWrapper wrapperStream( fileStream );
	Writer<OStreamWrapper> writer( wrapperStream );
	document.Accept( writer );
	fileStream.close();
	return true;
}

Document JSON::ParseFile( const std::string& file )
{
	Document document;

	std::ifstream fileStream( "Resources\\JSON\\" + file );
	if ( !fileStream.is_open() )
	{
		document.SetNull();
	}
	else
	{
		IStreamWrapper streamWrapper( fileStream );
		document.ParseStream( streamWrapper );
		if ( document.HasParseError() )
			document.SetNull();
		fileStream.close();
	}

	return document;
}