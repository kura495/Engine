#pragma once
#include "Base/PipelineState/IPipelineStateObject.h"
#include "Base/Manager/PSO/PSOManager.h"

class SpritePSO : public IPipelineStateObject
{
public:

	void ShaderCompile() override;
	void CreateRootSignature() override;
	void CreateInputLayOut() override;
	void CreateBlendState() override;
	void CreateRasterizarState() override;
	void CreatePipelineStateObject() override;

private:

};