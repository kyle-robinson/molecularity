#include "stdafx.h"
#include "JSON_Helper.h"


//gets all game objects out of a file
vector<JSON_LOADER::ModdleData> JSON_LOADER::LoadGameObjects(string fileName)
{
	Document d;	
	vector<ModdleData> Data;
	ModdleData ObjectData;
	
		d = ParseFile(fileName);
		if (!d.IsNull()) {
			//load from file
			for (Value& GameObject : d["GameObjects"].GetArray()) {
				//Name of object
				if (CheckDataIsThere<Value>("Name", GameObject))
				{
					ObjectData.ObjectName = GameObject["Name"].GetString();

				}
				//object file data
				if (CheckDataIsThere<Value>("FileName", GameObject))
				{
					ObjectData.FileName = GameObject["FileName"].GetString();
				}

				//posistion data
				if (CheckDataIsThere<Value>("Position", GameObject)) {
					ObjectData.Position = { GameObject["Position"][0].GetFloat(),GameObject["Position"][1].GetFloat(),GameObject["Position"][2].GetFloat() };
				}
				else if (CheckDataIsThere<Value>("PosX", GameObject)&& CheckDataIsThere<Value>("PosY", GameObject)&& CheckDataIsThere<Value>("PosZ", GameObject)) {

					ObjectData.Position.x = GameObject["PosX"][0].GetFloat();
					ObjectData.Position.y = GameObject["PosY"][0].GetFloat();
					ObjectData.Position.z = GameObject["PosZ"][0].GetFloat();
				}
				else if (CheckDataIsThere<Value>("PositionX", GameObject) && CheckDataIsThere<Value>("PositionY", GameObject) && CheckDataIsThere<Value>("PositionZ", GameObject)) {
					ObjectData.Position.x = GameObject["PositionX"][0].GetFloat();
					ObjectData.Position.y = GameObject["PositionY"][0].GetFloat();
					ObjectData.Position.z = GameObject["PositionZ"][0].GetFloat();
				}
				else
				{
					ObjectData.Position.x = 0;
					ObjectData.Position.y = 0;
					ObjectData.Position.z = 0;
				}
				
				//scale data
				if (CheckDataIsThere<Value>("Scale", GameObject)) {
					ObjectData.Scale = { GameObject["Scale"][0].GetFloat(),GameObject["Scale"][1].GetFloat(),GameObject["Scale"][2].GetFloat() };
				}
				else if (CheckDataIsThere<Value>("ScaleX", GameObject) && CheckDataIsThere<Value>("ScaleY", GameObject) && CheckDataIsThere<Value>("ScaleZ", GameObject)) {

					ObjectData.Scale.x = GameObject["ScaleX"][0].GetFloat();
					ObjectData.Scale.y = GameObject["ScaleY"][0].GetFloat();
					ObjectData.Scale.z = GameObject["ScaleZ"][0].GetFloat();
				}
				else
				{
					ObjectData.Scale.x = 0;
					ObjectData.Scale.y = 0;
					ObjectData.Scale.z = 0;
				}

				if (CheckDataIsThere<Value>("Rotation", GameObject)) {
					ObjectData.Rotation = { GameObject["Rotation"][0].GetFloat(),GameObject["Rotation"][1].GetFloat(),GameObject["Rotation"][2].GetFloat() };
				}
				else if (CheckDataIsThere<Value>("RotX", GameObject) && CheckDataIsThere<Value>("RotY", GameObject) && CheckDataIsThere<Value>("RotZ", GameObject)) {

					ObjectData.Rotation.x = GameObject["RotX"][0].GetFloat();
					ObjectData.Rotation.y = GameObject["RotY"][0].GetFloat();
					ObjectData.Rotation.z = GameObject["RotZ"][0].GetFloat();
				}
				else if (CheckDataIsThere<Value>("RotationX", GameObject) && CheckDataIsThere<Value>("RotationY", GameObject) && CheckDataIsThere<Value>("RotationZ", GameObject)) {
					ObjectData.Rotation.x = GameObject["RotationX"][0].GetFloat();
					ObjectData.Rotation.y = GameObject["RotationY"][0].GetFloat();
					ObjectData.Rotation.z = GameObject["RotationZ"][0].GetFloat();
				}
				else
				{
					ObjectData.Rotation.x = 0;
					ObjectData.Rotation.y = 0;
					ObjectData.Rotation.z = 0;
				}
				Data.push_back(ObjectData);
			}
		}

	//check if data is loaded 
	if (Data.size()==0)
	{
		//error
		
		OutputDebugStringA("Error: no data found");
		
	}
	
		return Data;
	
}
//load Text
vector<JSON_LOADER::TextData> JSON_LOADER::LoadTextDataItems(string FileName)
{
	Document document = ParseFile(FileName);
	
	vector<TextData> Data;

	//load data

	//load Data in data
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object)
	{
		for (Value& GameObject : document[Object->name.GetString()].GetArray()) {
			
				
				//stor strings in struct and vector
				TextData textData;
				if(CheckDataIsThere<Value>("Name", GameObject))
				textData.Name = GameObject["Name"].GetString();
				if(CheckDataIsThere<Value>("Text",GameObject))
				textData.Name = GameObject["Text"].GetString();
				Data.push_back(textData);
			

		}
	}

	//error checks
	//check if data is loaded 
	if (Data.size() == 0)
	{
		//error
		OutputDebugStringA("Error: no data found");
		
	}
	
	return Data;
}


//load all settings
vector<JSON_LOADER::SettingData>  JSON_LOADER::LoadSettings()
{
	
	
		
		vector<SettingData>Settings;
		string p;
		Document document = ParseFile("Settings.json");
		//load Data in data
		for (Value::ConstMemberIterator Object = document.MemberBegin();
			Object != document.MemberEnd(); ++Object) 
		{
			if (Object->value.IsArray()) {
				for (Value& GameObject : document[Object->name.GetString()].GetArray()) {
					for (Value::ConstMemberIterator itr = GameObject.MemberBegin();
						itr != GameObject.MemberEnd(); ++itr)
					{
						//getData
						SettingData Setting;

						string c = itr->name.GetString();
						Setting.Name = c;
						//stor strings in struct and vector
					
						Setting.Setting = GetDataAny(itr);
			
						Settings.push_back(Setting);
					}
				}
			}
			else
			{
			
						SettingData Setting;
						Setting.Name = Object->name.GetString();
						Setting.Setting = GetDataAny(Object);
						Settings.push_back(Setting);
			}

		}

		return Settings;
	
	

	
	
}


//get data as string
vector<string> JSON_LOADER::LoadFileData(string fileName)
{
	vector<string>Data;
	Document document = ParseFile(fileName);
	//load Data in string format
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object)
	{
		if(Object->value.IsArray()){
			for (Value& GameObject : document[Object->name.GetString()].GetArray()) {
				for (Value::ConstMemberIterator itr = GameObject.MemberBegin();
					itr != GameObject.MemberEnd(); ++itr)
				{

					//store strings in struct and vector
					Data.push_back(GetData(itr).second);
				}
			}
		
		}
		else
		{
			
			Data.push_back(GetData(Object).second);
		}
	}
	return Data;
}
//get data and there names as string 
vector<pair<string, string>> JSON_LOADER::LoadFileDataAndName(string fileName)
{
	vector<pair<string, string>>Data;
	Document document = ParseFile(fileName);
	//load Data in string format
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object)
	{
		if (Object->value.IsArray()) {
			for (Value& GameObject : document[Object->name.GetString()].GetArray()) {
				for (Value::ConstMemberIterator itr = GameObject.MemberBegin();
					itr != GameObject.MemberEnd(); ++itr)
				{

					Data.push_back(GetData(itr));
				}
			}

		}
		else
		{
			
			Data.push_back(GetData(Object));
		}
	}
	return Data;
	
}
//get data as string
pair<string, string> JSON_LOADER::GetData(Value::ConstMemberIterator Value)
{

	string StringData;
	switch (Value->value.GetType())
	{
	case(kStringType):
		StringData = Value->value.GetString();

		break;
	case(kNumberType):
		if (Value->value.IsInt()) {
			StringData = to_string(Value->value.GetInt());

		}
		else if (Value->value.IsUint()) {
			StringData = to_string(Value->value.GetUint());

		}
		else
		{
			StringData = to_string(Value->value.GetFloat());

		}
		break;

	case(kArrayType):
		
		break;
	case(kFalseType):
		StringData = "False";
		break;
	case(kTrueType):
		StringData = "True";
		break;
	default:
		break;
	}


	//stor strings in struct and vector
	
	string Name = Value->name.GetString();
	pair<string, string> a(Name, StringData);
	return a;
}

JSON_LOADER::DataFromFile JSON_LOADER::GetDataAny(Value::ConstMemberIterator Value)
{
	DataFromFile data;
	string p;
	switch (Value->value.GetType())
	{
	case(kStringType):
		p = Value->value.GetString();
		data = p;
		break;
	case(kNumberType):
		if (Value->value.IsInt()) {
			data = Value->value.GetInt();

		}
		else if (Value->value.IsDouble()) {
			data = Value->value.GetDouble();

		}
		else
		{
			data = Value->value.GetFloat();
		}
		break;

	case(kArrayType):

		break;
	case(kFalseType):
	case(kTrueType):
		data = Value->value.GetBool();
		break;
	default:
		break;
	}
	return data;
}

bool JSON_LOADER::StoreFile(string fileName, Document& d)
{
	//back to file
	std::ofstream t("Resources\\JSON\\" + fileName);
	OStreamWrapper s(t);
	Writer<OStreamWrapper> writer(s);
	d.Accept(writer);

	t.close();

	return true;
}

Document JSON_LOADER::ParseFile(string File)
{
	Document d;
	
		HRESULT hr;
		std::ifstream t("Resources\\JSON\\" + File);
		if (!t.is_open()) {
			//error
			d.SetNull();
		}
		else
		{
			IStreamWrapper isw(t);

			d.ParseStream(isw);
			if (d.HasParseError()) {
				//error
				d.SetNull();
			}
			t.close();
		}

	return d;
}