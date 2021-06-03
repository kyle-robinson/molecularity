#include "stdafx.h"
#include "TextLoader.h"

TextLoader::TextLoader()
{
	TextFile = "Text_Eng.json";
}

TextLoader::~TextLoader()
{
}
//get text data from json
//node is the section of the json file that is needed
std::vector<JSON::TextData> TextLoader::LoadText(std::string Node)
{
	return  JSON::LoadTextDataItems(TextFile, Node);
}
//convert texdata vector to string
map<string, string> TextLoader::ConvertToMap(vector<JSON::TextData> TextData)
{
	map<string, string> TextFile;
	for (auto& Text : TextData) {
		TextFile[Text.Name] = Text.Text;
	}

	return TextFile;
}
//change file useing full file name
void TextLoader::ChangeTextFile(std::string NewTextFile)
{
	TextFile = NewTextFile;
	EventSystem::Instance()->AddEvent(EVENTID::ChangeLanguageEvent);
}
//change file with langae code eg Eng,Fr,es
void TextLoader::ChangeLang(std::string LangCode)
{
	TextFile = "Text_";
	TextFile.append(LangCode);
	TextFile.append(".json");
	EventSystem::Instance()->AddEvent(EVENTID::ChangeLanguageEvent);
}

TextLoader* TextLoader::Instance()
{
	static TextLoader textLoader;
	return &textLoader;
}

