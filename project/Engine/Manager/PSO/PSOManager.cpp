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
	//PSOの初期化
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
	//PSOをリストに追加
	AddPipeline(standard.GetPSO(),PipelineType::Model);
	AddPipeline(skinning.GetPSO(),PipelineType::Skinning);
	AddPipeline(dissolve.GetPSO(),PipelineType::SkinningDissolve);
	AddPipeline(particlePipeLine.GetPSO(),PipelineType::Particle);
	AddPipeline(sprite.GetPSO(), PipelineType::Sprite);
	AddPipeline(drawLinePSO.GetPSO(),PipelineType::DrawLine);
	AddPipeline(wireFlame.GetPSO(),PipelineType::WireFlame);
	AddPipeline(cubeMap.GetPSO(),PipelineType::CubeMap);

	AddPipeline(postProsessPSO.GetPSO(), PostProsessType::PostProsessPSO);
	AddPipeline(gaussianFilter.GetPSO(), PostProsessType::GaussianFilter);
	AddPipeline(grayScale.GetPSO(), PostProsessType::GrayScale);
	AddPipeline(vinette.GetPSO(), PostProsessType::Vinette);
	AddPipeline(luminanceBasedOutline.GetPSO(), PostProsessType::LuminanceBasedOutline);
	AddPipeline(rGBtoHSV.GetPSO(), PostProsessType::RGBtoHSV);


}
