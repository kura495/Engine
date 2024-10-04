#pragma once

#include "PipelineState/IPipelineStateObject.h"
#include "Manager/PSO/PSOManager.h"

class ParticlePipeLine : public IPipelineStateObject
{
public:
	void ShaderCompile()override;
	void CreateRootSignature()override;
	void CreateInputLayOut()override;
	void CreateBlendState()override;
	void CreateRasterizarState()override;
	void CreatePipelineStateObject()override;
	
private:

};