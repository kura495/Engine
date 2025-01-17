#pragma once
//パイプラインマネージャークラス//
#include <map>

#include "Common/DirectX/DirectXCommon.h"

#include "PipelineState/Standard/Standard.h"
#include "PipelineState/PostProsess/PostProsessPSO.h"
#include "PipelineState/Skinning/Skinning.h"
#include "PipelineState/DrawLine/DrawLinePSO.h"
#include "PipelineState/WireFlame/WireFlame.h"
#include "PipelineState/GaussianFilter/GaussianFilter.h"
#include "PipelineState/GrayScale/GrayScale.h"
#include "PipelineState/Vignetting/Vignetting.h"
#include "PipelineState/LuminanceBasedOutline/LuminanceBasedOutline.h"
#include "PipelineState/RGBtoHSV/RGBtoHSV.h"
#include "PipelineState/CubeMap/CubeMap.h"
#include "PipelineState/Dissolve/Dissolve.h"
#include "PipelineState/Sprite/SpritePSO.h"
#include "Engine/ParticleSystem/PipeLine/ParticlePipeLine.h"

class PSOManager
{
public:
	static PSOManager* GetInstance();
	
	void Initalize();

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <param name="Type">PipelineTypeを選択</param>
	/// <returns>ID3D12RootSignature</returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(PipelineType Type) { return Pipeline_[Type].rootSignature.Get(); }
	/// <summary>
	/// パイプラインステートを選択
	/// </summary>
	/// <param name="Type">PipelineTypeを選択</param>
	/// <returns>ID3D12PipelineState</returns>
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(PipelineType Type) { return Pipeline_[Type].graphicsPipelineState.Get(); }
	/// <summary>
	/// PSOManagerにPipelineStateObjectを登録
	/// </summary>
	/// <param name="Pipeline">PipelineStateObjectを代入</param>
	/// <param name="Type">PipelineTypeを選択</param>
	void AddPipeline(PipelineStateObject Pipeline, PipelineType Type) {
		Pipeline_[Type] = Pipeline;
	}

private:

	std::map<PipelineType,PipelineStateObject> Pipeline_;

};
