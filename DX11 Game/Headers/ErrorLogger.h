#pragma once
#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H

#include <Windows.h>
#include "COMException.h"

class ErrorLogger
{
public:
	static void Log( const std::string& message ) noexcept;
	static void Log( HRESULT hr, const std::string& message ) noexcept;
	static void Log( HRESULT hr, const std::wstring& message ) noexcept;
	static void Log( COMException& exception ) noexcept;
};

#endif