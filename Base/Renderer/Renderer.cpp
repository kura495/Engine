#include "Renderer.h"

ViewProjection Renderer::viewProjection;
std::vector<DrawModelData> Renderer::drawModelData_;
std::vector<DrawSkinningData> Renderer::drawModelSkinningData_;
std::vector<DrawLineData> Renderer::drawLineData_;
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
	//標準描画
	ChangePipeline(PipelineType::Standerd);
	///描画
	for (DrawModelData model : drawModelData_) {
		model.modelData->Draw(*model.world_);
	}
	//中身を消す
	drawModelData_.clear();

	//Skinning
	ChangePipeline(PipelineType::Skinning);
	///描画
	for (DrawSkinningData model : drawModelSkinningData_) {
		model.modelData->SkinDraw(*model.world_,*model.skinCluster);
	}
	//中身を消す
	drawModelSkinningData_.clear();

	//ライン描画
	ChangePipeline(PipelineType::DrawLine);
	///描画
	for (DrawLineData model : drawLineData_) {
		model.lineData->Draw(*model.world_);
	}
	//中身を消す
	drawLineData_.clear();
}

void Renderer::AddModelData( Model& model, WorldTransform& world)
{
	DrawModelData result;
	result.modelData = &model;
	result.world_ = &world;
	drawModelData_.push_back(result);
}

void Renderer::AddModelSkinningData(Model& model, WorldTransform& world,SkinCluster& skinCluster)
{
	DrawSkinningData result;
	result.modelData = &model;
	result.world_ = &world;
	result.skinCluster = &skinCluster;
	drawModelSkinningData_.push_back(result);
}

void Renderer::AddLineData(Line& line, WorldTransform& world)
{
	DrawLineData result;
	result.lineData = &line;
	result.world_ = &world;
	drawLineData_.push_back(result);
}

void Renderer::ChangePipeline(PipelineType Type)
{
	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(Type).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(Type).Get());
}
