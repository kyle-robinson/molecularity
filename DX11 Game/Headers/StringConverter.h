#pragma once
#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H

#include <string>

class StringConverter
{
public:
	static std::wstring StringToWide( const std::string& narrow ) noexcept;
	static std::string StringToNarrow( const std::wstring& wide ) noexcept;
	static std::string GetDirectoryFromPath( const std::string& filePath );
	static std::string GetFileExtension( const std::string& fileName );
};

#endif