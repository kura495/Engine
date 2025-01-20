#pragma once
/*シェーダーコンパイルクラス*/
#include "Common/DirectX/DirectXCommon.h"

class ShaderCompiler
{
public:

	static ShaderCompiler* GetInstance();

	void Initalize();
	/// <summary>
	/// シェーダーをコンパイルする関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="profile">シェーダープロファイル</param>
	/// <returns></returns>
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);

private:
	ShaderCompiler() = default;
	~ShaderCompiler() = default;
	ShaderCompiler(const ShaderCompiler& obj) = delete;
	ShaderCompiler& operator=(const ShaderCompiler& obj) = delete;

	void MakeDXC();
	
	HRESULT hr;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;

	IDxcBlob* shaderBlob = nullptr;
	IDxcBlobUtf8* shaderError = nullptr;
};