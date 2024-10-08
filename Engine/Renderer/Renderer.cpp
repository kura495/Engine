#include "Renderer.h"
#include "ParticleSystem/ParticleSystem.h"

ViewProjection Renderer::viewProjection;
std::vector<DrawModelData> Renderer::drawModelData_;
std::vector<DrawSkinningData> Renderer::drawModelSkinningData_;
std::vector<DrawSkinningDissolveData> Renderer::drawModelSkinningDissolveData_;
std::vector<DrawLineData> Renderer::drawLineData_;
std::vector<DrawModelData> Renderer::drawWireFlameData_;
std::vector<DrawSpriteData> Renderer::drawSpriteData_;
std::vector<DrawParticleData> Renderer::drawParticleData_;
;
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

	viewproj.Initialize();
	viewProjection = viewproj;

	skyBox.Init();
	Texture = TextureManager::GetInstance()->LoadTexture("resources/rostock_laage_airport_4k.dds");
	skyBox.TextureHandle = Texture;
	cubeWorld_.Initialize();
	cubeWorld_.transform.scale *= 100;
	cubeWorld_.Update();

	dissolveTexture = TextureManager::GetInstance()->LoadTexture("resources/rostock_laage_airport_4k.dds");

}

void Renderer::Draw()
{

	//CubeMap
	ChangePipeline(PipelineType::CubeMap);
	//skyBox.Draw(cubeWorld_);



	
	//標準描画
	ChangePipeline(PipelineType::Standerd);
	///描画
	for (DrawModelData model : drawModelData_) {
		model.modelData->Draw(*model.world_,Texture);
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

	//SkinningとDissolve
	ChangePipeline(PipelineType::SkinningDissolve);
	///描画
	for (DrawSkinningDissolveData model : drawModelSkinningDissolveData_) {
		model.modelData->SkinDissolveDraw(*model.world_,*model.skinCluster, Texture, model.DissolveValue);
	}
	//中身を消す
	drawModelSkinningDissolveData_.clear();

	//ライン描画
	ChangePipeline(PipelineType::DrawLine);
	///描画
	for (DrawLineData model : drawLineData_) {
		model.lineData->Draw(*model.world_);
	}
	//中身を消す
	drawLineData_.clear();
	//ワイヤーフレーム描画
	ChangePipeline(PipelineType::WireFlame);
	///描画
	for (DrawModelData model : drawWireFlameData_) {
		model.modelData->WireFlameDraw(*model.world_);
	}
	//中身を消す
	drawWireFlameData_.clear();

	//Particle描画
	ChangePipeline(PipelineType::Particle);
	///描画
	for (DrawParticleData model : drawParticleData_) {
		model.particle->Draw(Renderer::viewProjection);
	}
	//中身を消す
	drawParticleData_.clear();

	//スプライト描画
	ChangePipeline(PipelineType::Sprite);
	///描画
	for (DrawSpriteData model : drawSpriteData_) {
		model.sprite->Draw(*model.world_);
	}
	//中身を消す
	drawSpriteData_.clear();


}

void Renderer::PostProsessDraw()
{
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

void Renderer::AddModelSkinningDissolveData(Model& model, WorldTransform& world, SkinCluster& skinCluster, float DissolveValue)
{
	DrawSkinningDissolveData result;
	result.modelData = &model;
	result.world_ = &world;
	result.skinCluster = &skinCluster;
	result.DissolveValue = DissolveValue;
	drawModelSkinningDissolveData_.push_back(result);
}

void Renderer::AddLineData(Line& line, WorldTransform& world)
{
	DrawLineData result;
	result.lineData = &line;
	result.world_ = &world;
	drawLineData_.push_back(result);
}

void Renderer::AddWireFlameData(Model& model, WorldTransform& world)
{
	DrawModelData result;
	result.modelData = &model;
	result.world_ = &world;
	drawWireFlameData_.push_back(result);
}

void Renderer::AddSpriteData(Sprite& sprite, WorldTransform& world)
{
	DrawSpriteData result;
	result.sprite = &sprite;
	result.world_ = &world;
	drawSpriteData_.push_back(result);
}

void Renderer::AddParticleData(ParticleSystem& particle)
{
	DrawParticleData result;
	result.particle = &particle;
	drawParticleData_.push_back(result);
}

void Renderer::ChangePipeline(PipelineType Type)
{
	commandList->SetGraphicsRootSignature(PSOManager_->GetRootSignature(Type).Get());
	commandList->SetPipelineState(PSOManager_->GetPipelineState(Type).Get());
}
