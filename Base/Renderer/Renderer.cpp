#include "Renderer.h"
Renderer* Renderer::GetInstance()
{
	static Renderer instance;
	return &instance;
}

void Renderer::Initalize()
{
	commandList = DirectXCommon::GetInstance()->GetcommandList();
	PSOManager_ = std::make_unique<PSOManager>();
	PSOManager_->Initalize();

}

void Renderer::Draw()
{
	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(PipelineType::Standerd).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(PipelineType::Standerd).Get());

	///描画

	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(PipelineType::Skinning).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(PipelineType::Skinning).Get());

	///描画

	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(PipelineType::DrawLine).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(PipelineType::DrawLine).Get());

	///描画

	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(PipelineType::PostProsessPSO).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(PipelineType::PostProsessPSO).Get());

	///描画

}

void Renderer::AddModelData(Model* model, WorldTransform* world)
{
	DrawModelData result;
	result.modelData = model;
	result.world_ = world;

}

void Renderer::AddModelSkinningData(const Model* model, WorldTransform* world, const SkinCluster* skinCluster)
{
}
