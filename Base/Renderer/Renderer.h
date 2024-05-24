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
struct DrawLineData {
	Line* lineData;
	WorldTransform* world_;
};

class Renderer
{
public:
	static Renderer* GetInstance();
	void Initalize();
	//void Update();
	void Draw();

	static void AddModelData(Model& model,WorldTransform& world);
	static void AddModelSkinningData(Model& model,WorldTransform& world,SkinCluster& skinCluster);
	static void AddLineData(Line& line,WorldTransform& world);

	 void ChangePipeline(PipelineType Type);

	static ViewProjection viewProjection;
private:
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer& obj) = delete;
	Renderer& operator=(const Renderer& obj) = delete;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList = nullptr;
	std::unique_ptr<PSOManager> PSOManager_ = nullptr;

	static std::vector<DrawModelData> drawModelData_;
	static std::vector<DrawSkinningData> drawModelSkinningData_;
	static std::vector<DrawLineData> drawLineData_;
};
