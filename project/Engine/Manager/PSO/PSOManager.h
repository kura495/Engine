#pragma once
//パイプラインマネージャークラス//
#include <map>

#include "Common/DirectX/DirectXCommon.h"

#include "Object/Model/PipeLine/ModelPSO.h"
#include "Object/Model/PipeLine/Skinning/Skinning.h"
#include "Object/Line/PipeLine/DrawLinePSO.h"
#include "Object/Model/PipeLine/WireFlame/WireFlame.h"
#include "Object/Sprite/PipeLine/SpritePSO.h"
#include "Object/CubeMap/Pipeline/CubeMap.h"

#include "PostProsess/Pipeline/PostProsessPSO.h"
#include "PostProsess/GaussianFilter/Pipeline/GaussianFilter.h"
#include "PostProsess/GrayScale/Pipeline/GrayScale.h"
#include "PostProsess/Vinette/Pipeline/Vinette.h"
#include "PostProsess/LuminanceBasedOutline/Pipeline/LuminanceBasedOutline.h"
#include "PostProsess/RGBtoHSV/Pipeline/RGBtoHSV.h"
#include "PostProsess/Dissolve/Pipeline/Dissolve.h"

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
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(PostProsessType Type) { return postProsess_[Type].rootSignature.Get(); }
	/// <summary>
	/// パイプラインステートを選択
	/// </summary>
	/// <param name="Type">PipelineTypeを選択</param>
	/// <returns>ID3D12PipelineState</returns>
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(PipelineType Type) { return Pipeline_[Type].graphicsPipelineState.Get(); }
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(PostProsessType Type) { return postProsess_[Type].graphicsPipelineState.Get(); }
	/// <summary>
	/// PSOManagerにPipelineStateObjectを登録
	/// </summary>
	/// <param name="Pipeline">PipelineStateObjectを代入</param>
	/// <param name="Type">PipelineTypeを選択</param>
	void AddPipeline(PipelineStateObject Pipeline, PipelineType Type) { Pipeline_[Type] = Pipeline; }
	void AddPipeline(PipelineStateObject Pipeline, PostProsessType Type) { postProsess_[Type] = Pipeline; }

private:

	std::map<PipelineType,PipelineStateObject> Pipeline_;
	std::map<PostProsessType,PipelineStateObject> postProsess_;

};
