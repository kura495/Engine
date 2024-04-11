#pragma once
#include <string>
#include <Windows.h>
#include <format>

void Log(const std::string& message);
std::wstring ConvertString(const std::string& str);
std::string ConvertString(const std::wstring& str);
std::string ConvertString(const wchar_t* in_wstr);
