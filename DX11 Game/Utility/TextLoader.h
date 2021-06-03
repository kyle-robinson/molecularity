#pragma once

#include <Utility/JSON_Helper.h>
using namespace std;
class TextLoader
{
public:
	

	std::vector<JSON::TextData> LoadText(std::string Node);
	std::map<std::string,std::string> ConvertToMap(std::vector<JSON::TextData>TextData);


	void ChangeTextFile(std::string NewTextFile);
	void ChangeLang(std::string LangCode);
	static TextLoader* Instance();

private:
	TextLoader();
	~TextLoader();

	std::string TextFile;

};


