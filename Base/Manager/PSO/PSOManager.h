#pragma once

#include <map>

#include "Common/DirectX/DirectXCommon.h"

#include "PipelineState/Standard/Standard.h"
#include "PipelineState/PostProsess/PostProsessPSO.h"
#include "PipelineState/Skinning/Skinning.h"
#include "PipelineState/DrawLine/DrawLinePSO.h"
#include "PipelineState/GaussianFilter/GaussianFilter.h"

class PSOManager
{
public:
	static PSOManager* GetInstance();
	
	void Initalize();
	//void Update();
	//void Draw();

	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(PipelineType Type) { return Pipeline_[Type].rootSignature.Get(); }
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(PipelineType Type) { return Pipeline_[Type].graphicsPipelineState.Get(); }

	void AddPipeline(PipelineStateObject Pipeline, PipelineType Type) {
		Pipeline_[Type] = Pipeline;
	}

private:

	std::map<PipelineType,PipelineStateObject> Pipeline_;

};
