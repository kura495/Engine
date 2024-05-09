#include "PSOManager.h"

PSOManager* PSOManager::GetInstance()
{
	static PSOManager instance;
	return &instance;
}

void PSOManager::Initalize()
{
ShaderCompiler* shaderCompiler = ShaderCompiler::GetInstance();
		shaderCompiler->Initalize();

	std::unique_ptr<Standard> standard = std::make_unique<Standard>();
	standard->Initalize();
	std::unique_ptr<PostProsessPSO> postProsessPSO = std::make_unique<PostProsessPSO>();
	postProsessPSO->Initalize();
	std::unique_ptr<Skinning> skinning = std::make_unique<Skinning>();
	skinning->Initalize();
	AddPipeline(standard->GetPSO(),PipelineType::Standerd);
	AddPipeline(postProsessPSO->GetPSO(),PipelineType::PostProsessPSO);
	AddPipeline(skinning->GetPSO(),PipelineType::Skinning);
}
