#include "stdafx.h"
#include "TextLoader.h"

TextLoader::TextLoader()
{
	TextFile = "Text_Eng.json";
}

TextLoader::~TextLoader() {}

// Get text data from json
// Node is the section of the json file that is needed
std::vector<JSON::TextData> TextLoader::LoadText( std::string Node )
{
	return  JSON::LoadTextDataItems( TextFile, Node );
}

// Convert texdata vector to string
std::unordered_map<std::string, std::string> TextLoader::ConvertToMap( std::vector<JSON::TextData> TextData )
{
	std::unordered_map<std::string, std::string> TextFile;
	for ( auto& Text : TextData )
		TextFile[Text.Name] = Text.Text;
	return TextFile;
}

// Change file using full file name
void TextLoader::ChangeTextFile( std::string NewTextFile )
{
	TextFile = NewTextFile;
	EventSystem::Instance()->AddEvent( EVENTID::ChangeLanguageEvent );
}

// Change file with language code e.g. Eng/Fr
void TextLoader::ChangeLang( std::string LangCode )
{
	TextFile = "Text_";
	TextFile.append( LangCode );
	TextFile.append( ".json" );
	EventSystem::Instance()->AddEvent( EVENTID::ChangeLanguageEvent );
}

TextLoader* TextLoader::Instance()
{
	static TextLoader textLoader;
	return &textLoader;
}