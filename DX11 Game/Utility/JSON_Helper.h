#pragma once
#include"rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include<variant>
using namespace rapidjson;
using namespace std;


namespace JSON_LOADER {
	typedef std::variant<int, string, bool, float, double> DataFromFile;
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
	//Get data by:
	// get<type>(array_name[Position_in_Array].Setting);
	DataFromFile Setting;

};

struct TextData {
	string Name;
	string Text;
	//add more data here eg position
};
enum CamraType
{
	Default,
	Static,
	Debug
};
struct CamraData {
	string Name;
	string type;
	DirectX::XMFLOAT3 Position;
	
};

enum LightType
{
	Point,
	directional,
	spotlight
};
struct LigthData {
	string Name;
	



};


	//load GameObjects get all data
	vector<ModdleData> LoadGameObjects(string fileName);
	
	//load text data get all data
	vector<TextData> LoadTextDataItems(string FileName);
	
	//Load Setting Files get all data
	vector<SettingData> LoadSettings();
	

	//Load one Node as string
	vector<string> LoadJSONNode(string JSONFIle, string Node,string DataNode = "");

	//Load all Nodes
	vector<string> LoadFileData(string fileName);
	vector<pair<string,string>> LoadFileDataAndName(string fileName);
	pair<string, string> GetData(Value::ConstMemberIterator Value);
	
	DataFromFile GetDataAny(Value::ConstMemberIterator Value);

	//add new Node
	template<typename DataType>
	void AddNode(string fileName, string nodeName, DataType Data) {

	}
	


	//get file
	Document ParseFile(string File);
	//store file
	bool StoreFile(string fileName, Document& d);
	
	//check node is ther
	template <typename DataFormat>
	bool CheckDataIsThere(string ObjectName, DataFormat& doc) {

		return doc.HasMember(ObjectName.c_str());;
	}

	//function to set string data 
 template <typename DataFormat> void addObject(DataFormat& d, string a,string data) {
		Document document;
		d[a.c_str()].SetString(data.c_str(), document.GetAllocator());
	}
	//function to set int data 
	template <typename DataFormat> void addObject(DataFormat& d, string a, int data) {
		Document document;
		d[a.c_str()].SetInt(data);
	}
	//function to set double data 
	template <typename DataFormat> void addObject(DataFormat& d, string a, double data) {
		Document document;
		d[a.c_str()].SetDouble(data);
	}
	//function to set float data 
	template <typename DataFormat> void addObject(DataFormat& d, string a, float data) {
		Document document;
		d[a.c_str()].SetFloat(data);
	}
	//function to set bool data 
	template <typename DataFormat> void addObject(DataFormat& d, string a, bool data) {
		Document document;
		d[a.c_str()].SetBool(data);
	}

	//Update Data: can update data by checking name data
	template <typename DataTypeToSet>
	void UpdateJSONItemEX(string JSONFIle, string Node, string DataNode, DataTypeToSet Data, string NameOfData)
	{
		//load document
		Document d = ParseFile(JSONFIle);
		auto b = Data;
		//set node
		if (d.HasMember(Node.c_str())) {
			if (d[Node.c_str()].IsArray()) {
				//load from file
				for (Value& Object : d[Node.c_str()].GetArray()) {
					if (DataNode != "") {
						if (Object.HasMember(DataNode.c_str())) {
							if (NameOfData != "") {
								if (Object["Name"].GetString() == NameOfData) {
									//chack type
									addObject<Value>(Object, DataNode, b);
								}
							}
							else {
								
								addObject<Value>(Object, DataNode, b);

							}
						}
					}
				}
			}
			else
			{
				addObject<Document>(d, Node, b);
				
			}
		}

		StoreFile(JSONFIle, d);

	}


	//will update nodes
	template<typename DataTypeToSet>
	void UpdateJSONItem(string JSONFIle, string Node, DataTypeToSet Data, string DataNode)
	{
		UpdateJSONItemEX<DataTypeToSet>(JSONFIle, Node, DataNode, Data, "");
	}

}



