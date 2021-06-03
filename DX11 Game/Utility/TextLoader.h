#pragma once
#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include "JSON_Helper.h"

/// <summary>
/// Handle the change between different languages and text files.
/// </summary>
class TextLoader
{
public:
	static TextLoader* Instance();
	void ChangeLang( std::string LangCode );
	void ChangeTextFile( std::string NewTextFile );

	std::vector<JSON::TextData> LoadText( std::string Node );
	std::map<std::string, std::string> ConvertToMap( std::vector<JSON::TextData>TextData );
private:
	TextLoader();
	~TextLoader();
	std::string TextFile;
};

#endif