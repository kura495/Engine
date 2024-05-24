#pragma once

#include "Common/DirectX/DirectXCommon.h"
#include "Manager/PSO/PSOManager.h"
#include "PipelineState/Standard/Standard.h"

#include "Base/Object/Model/Model.h"
#include "Base/WorldTransform/WorldTransform.h"
struct DrawModelData {
	Model* modelData;
	WorldTransform* world_;
};
struct DrawSkinningData {
	Model* modelData;
	WorldTransform* world_;
	SkinCluster* skinCluster;
};
class Renderer
{
public:
	static Renderer* GetInstance();
	void Initalize();
	//void Update();
	void Draw();

	void AddModelData(const Model* model,WorldTransform* world);
	void AddModelSkinningData(const Model* model,WorldTransform* world,const SkinCluster* skinCluster);
private:
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer& obj) = delete;
	Renderer& operator=(const Renderer& obj) = delete;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList = nullptr;
	std::unique_ptr<PSOManager> PSOManager_ = nullptr;

	std::vector<DrawModelData> drawModelData_;
	std::vector<DrawModelData> drawModelSkinningData_;
};
