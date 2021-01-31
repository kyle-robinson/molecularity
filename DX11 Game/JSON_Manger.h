#pragma once
#include <fstream>
#include <string>
#include<Windows.h>
#include"rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;
using namespace std;
//structs to store loaded object 
struct ModdleData
{
	string ObjectName;
	string FileName;
	string Texture;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT3 Rotation;
};

struct SettingData
{
	string Name;
	string Setting;

};


namespace JSON_LOADER {
	//load GameObjects
	vector<ModdleData> LoadGameObjects(string fileName);
	

	void LoadTextData(string FileName);

	//Load Setting Files
	void LoadSettings();
	void UpdateSettings(string SettingName);

	//Load other ithems
	vector<string> LoadJSONNode(string JSONFIle, string Node,string DataNode = "");
	void UpdateJSONItem(string JSONFIle, string Node, string DataNode = "");


	//checks
	string CheckType(Document& Document,string Node);
	string CheckType(Value& Document, string Node);
	bool CheckIsThere(string ObjectName, Value& doc);



	Document ParseFile(string File);
}

//TODO Json any node parser