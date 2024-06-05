#include "PSOManager.h"

PSOManager* PSOManager::GetInstance()
{
	static PSOManager instance;
	return &instance;
}

void PSOManager::Initalize()
{
	//　TODO : ここでやるべきではないのでInitをどこかへ移動したい
	ShaderCompiler* shaderCompiler = ShaderCompiler::GetInstance();
	shaderCompiler->Initalize();

	Standard standard;
	standard.Initalize();
	Skinning skinning;
	skinning.Initalize();
	DrawLinePSO drawLinePSO;
	drawLinePSO.Initalize();
	PostProsessPSO postProsessPSO;
	postProsessPSO.Initalize();

	AddPipeline(standard.GetPSO(),PipelineType::Standerd);
	AddPipeline(skinning.GetPSO(),PipelineType::Skinning);
	AddPipeline(drawLinePSO.GetPSO(),PipelineType::DrawLine);
	AddPipeline(postProsessPSO.GetPSO(),PipelineType::PostProsessPSO);
}
