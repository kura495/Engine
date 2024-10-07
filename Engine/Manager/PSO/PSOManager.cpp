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
	Dissolve dissolve;
	dissolve.Initalize();
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
	Vignetting vinette;
	vinette.Initalize();
	LuminanceBasedOutline luminanceBasedOutline;
	luminanceBasedOutline.Initalize();
	RGBtoHSV rGBtoHSV;
	rGBtoHSV.Initalize();
	CubeMap cubeMap;
	cubeMap.Initalize();
	ParticlePipeLine particlePipeLine;
	particlePipeLine.Initalize();
	SpritePSO sprite;
	sprite.Initalize();

	AddPipeline(standard.GetPSO(),PipelineType::Standerd);
	AddPipeline(skinning.GetPSO(),PipelineType::Skinning);
	AddPipeline(dissolve.GetPSO(),PipelineType::SkinningDissolve);
	AddPipeline(particlePipeLine.GetPSO(),PipelineType::Particle);
	AddPipeline(standard.GetPSO(), PipelineType::Sprite);
	AddPipeline(drawLinePSO.GetPSO(),PipelineType::DrawLine);
	AddPipeline(wireFlame.GetPSO(),PipelineType::WireFlame);
	AddPipeline(postProsessPSO.GetPSO(),PipelineType::PostProsessPSO);
	AddPipeline(gaussianFilter.GetPSO(),PipelineType::GaussianFilter);
	AddPipeline(grayScale.GetPSO(),PipelineType::GrayScale);
	AddPipeline(vinette.GetPSO(),PipelineType::Vinette);
	AddPipeline(luminanceBasedOutline.GetPSO(),PipelineType::LuminanceBasedOutline);
	AddPipeline(rGBtoHSV.GetPSO(),PipelineType::RGBtoHSV);
	AddPipeline(cubeMap.GetPSO(),PipelineType::CubeMap);

}
