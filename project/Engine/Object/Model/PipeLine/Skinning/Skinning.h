#pragma once
#include "PipelineState/IPipelineStateObject.h"
#include "Manager/PSO/PSOManager.h"

class Skinning : public IPipelineStateObject
{
public:

	void ShaderCompile() override;
	void CreateRootSignature() override;
	void CreateInputLayOut() override;
	void CreateBlendState() override;
	void CreateRasterizarState() override;
	void CreatePipelineStateObject() override;

private:
	D3D12_INPUT_ELEMENT_DESC inputElementarrayDescs[5]{};
};
#pragma once
