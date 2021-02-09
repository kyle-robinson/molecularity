#include "stdafx.h"
#include "JSON_Helper.h"


//gets all game objects out of a file
vector<ModdleData> JSON_LOADER::LoadGameObjects(string fileName)
{
	Document d = ParseFile(fileName);
	
	vector<ModdleData> Data;
	ModdleData ObjectData;
	

	//load from file
	for (Value& GameObject : d["GameObjects"].GetArray()) {

		if( CheckIsThere("Name", GameObject))
		{
			ObjectData.ObjectName = GameObject["Name"].GetString();
			
		}
		if (CheckIsThere("FileName", GameObject))
		{
			ObjectData.FileName = GameObject["FileName"].GetString();
		}
		if (CheckIsThere("Texture", GameObject))
		{
			ObjectData.Texture = GameObject["Texture"].GetString();
		}

		if(CheckIsThere("Positon", GameObject)) {
			ObjectData.Position = { GameObject["Positon"][0].GetFloat(),GameObject["Positon"][1].GetFloat(),GameObject["Positon"][2].GetFloat() };
		}
		if (CheckIsThere("Scale", GameObject)) {
			ObjectData.Scale = { GameObject["Scale"][0].GetFloat(),GameObject["Scale"][1].GetFloat(),GameObject["Scale"][2].GetFloat() };
		}
		if (CheckIsThere("Rotation", GameObject)) {
			ObjectData.Rotation = { GameObject["Rotation"][0].GetFloat(),GameObject["Rotation"][1].GetFloat(),GameObject["Rotation"][2].GetFloat() };
		}
		Data.push_back(ObjectData);
	}


	//check if data is loaded 
	if (Data.size()==0)
	{
		//error
	}
	else {
		return Data;
	}
	
}

void JSON_LOADER::LoadSettings()
{
	vector<SettingData>Settings;
	Document document = ParseFile("Settings.json");
	//load Data in string format
	for (Value::ConstMemberIterator Object = document.MemberBegin();
		Object != document.MemberEnd(); ++Object) 
	{
	
		for (Value& GameObject :document[Object->name.GetString()].GetArray()) {
			for (Value::ConstMemberIterator itr = GameObject.MemberBegin();
				itr != GameObject.MemberEnd(); ++itr)
			{
				//getData
				string c = itr->name.GetString();
				string b;
				switch (itr->value.GetType())
				{
				case(kStringType):
					b = itr->value.GetString();

					break;
				case(kNumberType):
					if (itr->value.IsInt()) {
						b = to_string(itr->value.GetInt());

					}
					else if (itr->value.IsUint()) {
						b = to_string(itr->value.GetUint());

					}
					else
					{
						b = to_string(itr->value.GetFloat());

					}
					break;

				case(kArrayType):

					break;
				case(kFalseType):
					b = "False";
					break;
				case(kTrueType):
					b = "True";
					break;
				default:
					break;
				}


				//stor strings in struct and vector
				SettingData Setting;
				Setting.Name = c;
				Setting.Setting = b;
				Settings.push_back(Setting);
			}

		}
	}

	//errors


	//return data
	
}

void JSON_LOADER::UpdateSettings(string SettingType, string SettingName, string UPdateData)
{
	

	//find setting
	UpdateJSONItemEX("Settings.json", SettingType, "", UPdateData, SettingName);


}

//only returns one value
vector<string> JSON_LOADER::LoadJSONNode(string JSONFIle, string Node, string DataNode)
{
	vector<string> Result;
	
	Value Data;
	
	Document d=ParseFile(JSONFIle);
	if (d.IsNull()) {
		Data.SetString("Error Empty File");
		Result.push_back(Data.GetString());
		return Result;
	}

	//check is there
	if (d.HasMember(Node.c_str())) {
		if (d[Node.c_str()].IsArray()) {
			//load from file
			for (Value& Object : d[Node.c_str()].GetArray()) {
				
				Data = CheckType(Object, DataNode);
				/*if (!Data.IsString()) {
					Data=to_string(Data.GetInt());
				}*/



				Result.push_back(Data.GetString());
			}
		}
		else
		{
			Data = CheckType(d, Node);
			Result.push_back(Data.GetString());
		}
	}

	//check if data is loaded 
	if (Result.size() == 0)
	{
		//error
	}
	else {

		OutputDebugStringA(Result[0].c_str());
		
		return Result;
	}
}

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

					string StringData;
					switch (itr->value.GetType())
					{
					case(kStringType):
						StringData = itr->value.GetString();

						break;
					case(kNumberType):
						if (itr->value.IsInt()) {
							StringData = to_string(itr->value.GetInt());

						}
						else if (itr->value.IsUint()) {
							StringData = to_string(itr->value.GetUint());

						}
						else
						{
							StringData = to_string(itr->value.GetFloat());

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


					Data.push_back(StringData);
				}
			}
		
		}
		else
		{
			string StringData;
			switch (Object->value.GetType())
			{
			case(kStringType):
				StringData = Object->value.GetString();

				break;
			case(kNumberType):
				if (itr->value.IsInt()) {
					StringData = to_string(itr->value.GetInt());

				}
				else if (itr->value.IsUint()) {
					StringData = to_string(itr->value.GetUint());

				}
				else
				{
					StringData = to_string(itr->value.GetFloat());

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
			Data.push_back(StringData);
		}
	}
	return Data;
}

void JSON_LOADER::UpdateJSONItem(string JSONFIle, string Node, string Data, string DataNode)
{
	UpdateJSONItemEX(JSONFIle, Node,"", Data, DataNode);
}

void JSON_LOADER::UpdateJSONItemEX(string JSONFIle, string Node, string DataName, string Data, string DataNode)
{

	Document d = ParseFile(JSONFIle);

	if (d.HasMember(Node.c_str())) {
		if (d[Node.c_str()].IsArray()) {
			//load from file
			for (Value& Object : d[Node.c_str()].GetArray()) {
				if (DataNode != "") {
					if (Object.HasMember(DataNode.c_str())) {
						if (DataName != "") {
							if (Object["Name"].GetString() == DataName)
								Object[DataNode.c_str()].SetString(Data.c_str(), d.GetAllocator());
						}
						else{
							Object[DataNode.c_str()].SetString(Data.c_str(), d.GetAllocator());
						}
					}
				}
			}
		}
		else
		{
			d[Node.c_str()].SetString(Data.c_str(), d.GetAllocator());
		}
	}

	std::ofstream t("JSON_File/" + JSONFIle);
	OStreamWrapper s(t);
	Writer<OStreamWrapper> writer(s);
	d.Accept(writer);

	t.close();

}

Value JSON_LOADER::CheckType(Document& Document, string Node)
{
	//check trype
	Value value;
	switch (Document[Node.c_str()].GetType())
	{
		case(kStringType):
	
			value.Set(Document[Node.c_str()].GetString());

			break;
		case(kNumberType):
			if (Document[Node.c_str()].IsInt()) {
				value.SetInt(Document[Node.c_str()].GetInt());
			}
			else if (Document[Node.c_str()].IsUint()) {
				value.SetUint(Document[Node.c_str()].GetUint());
			}
			else
			{
				value.SetDouble(Document[Node.c_str()].GetDouble());
			}
			break;

		case(kArrayType):

			break;
		default:

			break;
	}
	return value;
}

Value JSON_LOADER::CheckType(Value& Document, string Node)
{
	Value value;
	string a;
	
	switch (Document[Node.c_str()].GetType())
	{
	case(kStringType):
		value.Set(Document[Node.c_str()].GetString());
		
		break;
	case(kNumberType):
		if (Document[Node.c_str()].IsInt()) {
			value.SetInt(Document[Node.c_str()].GetInt());
		}
		else if (Document[Node.c_str()].IsUint()) {
			value.SetUint(Document[Node.c_str()].GetUint());
		}
		else
		{
			value.SetDouble(Document[Node.c_str()].GetDouble());
		}
		break;
	case(kArrayType):

		break;

	default:

		break;
	}
	return value;
}

bool JSON_LOADER::CheckIsThere(string ObjectName, Value& doc)
{

	return doc.HasMember(ObjectName.c_str());

}

Document JSON_LOADER::ParseFile(string File)
{
	Document d;
	std::ifstream t("JSON_File/" + File);
	if (!t.is_open()) {
		//error
		OutputDebugStringA("Not Open");
	}
	else
	{
		IStreamWrapper isw(t);

		d.ParseStream(isw);
		if (d.HasParseError()) {
			//error
			d = NULL;
		}
	}

	t.close();

	return d;
}
