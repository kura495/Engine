#pragma once
/*ログクラス*/
#include <string>
#include <Windows.h>
#include <format>
#include <cassert>
/// <summary>
/// 出力欄にメッセージを出力
/// </summary>
/// <param name="message">文字列</param>
void Log(const std::string& message);
/// <summary>
/// 文字列をワイド文字列にコンバート
/// </summary>
/// <param name="str">文字列</param>
/// <returns>std::wstring</returns>
std::wstring ConvertString(const std::string& str);
/// <summary>
/// ワイド文字列を文字列にコンバート
/// </summary>
/// <param name="str">文字列</param>
/// <returns>std::string</returns>
std::string ConvertString(const std::wstring& str);
/// <summary>
/// ワイド文字配列を文字列にコンバート
/// </summary>
/// <param name="in_wstr">ワイド文字配列</param>
/// <returns>std::string</returns>
std::string ConvertString(const wchar_t* in_wstr);
/// <summary>
/// 文字列をワイド文字列にコンバート
/// </summary>
/// <param name="str">文字列</param>
/// <returns>WCHAR*</returns>
WCHAR* ConvertMultiBype(const std::string& str);