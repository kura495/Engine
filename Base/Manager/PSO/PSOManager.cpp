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
	GaussianFilter gaussianFilter;
	gaussianFilter.Initalize();
	WireFlame wireFlame;
	wireFlame.Initalize();
	GrayScale grayScale;
	grayScale.Initalize();

	AddPipeline(standard.GetPSO(),PipelineType::Standerd);
	AddPipeline(skinning.GetPSO(),PipelineType::Skinning);
	AddPipeline(drawLinePSO.GetPSO(),PipelineType::DrawLine);
	AddPipeline(wireFlame.GetPSO(),PipelineType::WireFlame);
	AddPipeline(postProsessPSO.GetPSO(),PipelineType::PostProsessPSO);
	AddPipeline(gaussianFilter.GetPSO(),PipelineType::GaussianFilter);
	AddPipeline(grayScale.GetPSO(),PipelineType::GrayScale);

}
