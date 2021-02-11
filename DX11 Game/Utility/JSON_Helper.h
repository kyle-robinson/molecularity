#pragma once
#include"rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

using namespace rapidjson;
using namespace std;


namespace JSON_LOADER {
//structs to store loaded object 
struct ModdleData
{
	string ObjectName;
	string FileName;
	/*string Texture;*/
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT3 Rotation;
};

struct SettingData
{
	string Name;
	string Setting;

};

struct TextData {
	string Name;
	string Text;
	//add more data here eg position
};


	//load GameObjects grabe all data
	vector<ModdleData> LoadGameObjects(string fileName);
	
	//load text data get all data
	vector<TextData> LoadTextDataItems(string FileName);
	
	//Load Setting Files get all data
	vector<SettingData> LoadSettings();
	void UpdateSettings(string SettingType,string SettingName,string UPdateData);



	//Load one Node
	vector<string> LoadJSONNode(string JSONFIle, string Node,string DataNode = "");

	//Load all Nodes
	vector<string> LoadFileData(string fileName);
	vector<pair<string,string>> LoadFileDataAndName(string fileName);

	//update file 
	void UpdateJSONItem(string JSONFIle, string Node, string Data,string DataNode = "");
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataName,string Data,string DataNode = "");
	void UpdateJSONItem(string JSONFIle, string Node, int Data, string DataNode = "");
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataName, int Data, string DataNode = "");
	void UpdateJSONItem(string JSONFIle, string Node, float Data, string DataNode = "");
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataName, float Data, string DataNode = "");
	void UpdateJSONItem(string JSONFIle, string Node, double Data, string DataNode = "");
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataName, double Data, string DataNode = "");
	void UpdateJSONItem(string JSONFIle, string Node, DirectX::XMFLOAT3 Data, string DataNode = "");
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataName, DirectX::XMFLOAT3 Data, string DataNode = "");

	//add new Node
	void AddNode(string fileName,string nodeName,string Data);
	void AddNode(string fileName, string nodeName, int Data);
	void AddNode(string fileName, string nodeName, float Data);
	void AddNode(string fileName, string nodeName, double Data);
	void AddNode(string fileName, string nodeName, DirectX::XMFLOAT3 Data);


	//need to rename
	Value CheckType(Document& Document,string Node);
	Value CheckType(Value& Document, string Node);
	
	//helpers
	Document ParseFile(string File);
	bool CheckIsThere(string ObjectName, Value& doc);

	string getDataAsString(Value dataToGet);
	//loop array
	//loop node


}
