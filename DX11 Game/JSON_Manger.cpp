#include "stdafx.h"
#include "JSON_Manger.h"

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
			ObjectData.ObjectName = CheckType(GameObject, "Name");
			/*ObjectData.ObjectName = GameObject["Name"].GetString();*/
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

//only returns strings
vector<string> JSON_LOADER::LoadJSONNode(string JSONFIle, string Node, string DataNode)
{
	vector<string> Result;

	Document d=ParseFile(JSONFIle);
	if (d.IsNull()) {
		Result.push_back("Error Empty File");
		return Result;
	}


	//check is there
	if (d.HasMember(Node.c_str())) {
		if (d[Node.c_str()].IsArray()) {
			//load from file
			for (Value& Object : d[Node.c_str()].GetArray()) {
				//check what object is
				string data = CheckType(Object, DataNode);
				//in vector
				Result.push_back(data);
			}
		}
		else
		{
			string data = CheckType(d, Node);
			//in vector
			Result.push_back(data);
		}
	}

	//check if data is loaded 
	if (Result.size() == 0)
	{
		//error
	}
	else {
		return Result;
	}
}

string JSON_LOADER::CheckType(Document& Document, string Node)
{
	//check trype
	string value;
	switch (Document[Node.c_str()].GetType())
	{
		case(kStringType):
	
			value = Document[Node.c_str()].GetString();

			break;
		case(kNumberType):
			if (Document[Node.c_str()].IsInt())
				value = to_string(Document[Node.c_str()].GetInt());
			else if (Document[Node.c_str()].IsUint())
				value = to_string(Document[Node.c_str()].GetUint());
			else
				value = to_string(Document[Node.c_str()].GetFloat());
			break;
		default:

			break;
	}
	return value;
}

string JSON_LOADER::CheckType(Value& Document, string Node)
{
	string value;
	switch (Document[Node.c_str()].GetType())
	{
	case(kStringType):

		value = Document[Node.c_str()].GetString();

		break;
	case(kNumberType):
		if (Document[Node.c_str()].IsInt())
			value = to_string(Document[Node.c_str()].GetInt());
		else if (Document[Node.c_str()].IsUint())
			value = to_string(Document[Node.c_str()].GetUint());
		else
			value = to_string(Document[Node.c_str()].GetDouble());
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
	std::ifstream t(File);
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
	return d;
}
