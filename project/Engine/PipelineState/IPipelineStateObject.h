#pragma once
/*パイプラインステートオブジェクト基底クラス*/
#include "ShaderCompiler/ShaderCompiler.h"

class IPipelineStateObject
{
public:

	~IPipelineStateObject() {};
	/// <summary>
	/// 必要な初期化処理を実行
	/// </summary>
	void Initalize() {
		
		ShaderCompile();
		CreateRootSignature();
		CreateInputLayOut();
		CreateBlendState();
		CreateRasterizarState();
		CreatePipelineStateObject();
	}
	/// <summary>
	/// シェーダーをコンパイル
	/// </summary>
	virtual void ShaderCompile() = 0;
	/// <summary>
	/// ルートシグネチャを設定
	/// </summary>
	virtual void CreateRootSignature() = 0;
	/// <summary>
	/// インプットレイアウトを設定
	/// </summary>
	virtual void CreateInputLayOut() = 0;
	/// <summary>
	/// ブレンドステートを設定
	/// </summary>
	virtual void CreateBlendState() = 0;
	/// <summary>
	/// ラスタライザステートを設定
	/// </summary>
	virtual void CreateRasterizarState() = 0;
	/// <summary>
	/// パイプラインステートオブジェクトを設定
	/// </summary>
	virtual void CreatePipelineStateObject() = 0;
	/// <summary>
	/// パイプラインステートオブジェクトを取得
	/// </summary>
	/// <returns>PipelineStateObject</returns>
	PipelineStateObject GetPSO() { return PipelineStateObject_; }

protected:
	HRESULT hr;

	IDxcBlob* vertexShaderBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	D3D12_BLEND_DESC blendDesc{};

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	PipelineStateObject PipelineStateObject_;

};
