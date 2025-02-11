#pragma once
/*描画を一つにまとめるクラス*/
#include "Common/DirectX/DirectXCommon.h"
#include "Manager/PSO/PSOManager.h"

#include "Object/Model/Model.h"
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"
#include "WorldTransform/WorldTransform.h"


#include "Object/SkyBox/SkyBox.h"

struct DrawModelData {
	Model* modelData;
	WorldTransform* world_;
};
struct DrawSkinningData {
	Model* modelData;
	WorldTransform* world_;
	SkinCluster* skinCluster;
};
struct DrawSkinningDissolveData {
	Model* modelData;
	WorldTransform* world_;
	SkinCluster* skinCluster;
	float DissolveValue;
};
struct DrawLineData {
	Line* lineData;
	WorldTransform* world_;
};
struct DrawSpriteData {
	Sprite* sprite;
	WorldTransform* world_;
};
class ParticleSystem;
struct DrawParticleData {
	ParticleSystem* particle;
};
class Renderer
{
public:
	static Renderer* GetInstance();
	void Initalize();
	void Update();
	void Draw();
	void PostProsessDraw();
	/// <summary>
	/// 通常のモデルデータを描画リストに追加
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="world">座標情報</param>
	static void AddModelData(Model& model,WorldTransform& world);
	/// <summary>
	/// スキンアニメーション付きのモデルデータを描画リストに追加
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="world">座標情報</param>
	/// <param name="skinCluster">スキンクラスター</param>
	static void AddModelSkinningData(Model& model,WorldTransform& world,SkinCluster& skinCluster);
	/// <summary>
	/// スキンアニメーション+ディゾルブのモデルデータを描画リストに追加
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="world">座標情報</param>
	/// <param name="skinCluster">スキンクラスター<</param>
	/// <param name="DissolveValue">ディゾルブの値</param>
	static void AddModelSkinningDissolveData(Model& model,WorldTransform& world,SkinCluster& skinCluster, float DissolveValue);
	/// <summary>
	/// ラインデータの描画リストに追加
	/// </summary>
	/// <param name="line">ラインデータ</param>
	/// <param name="world">座標情報</param>
	static void AddLineData(Line& line,WorldTransform& world);
	/// <summary>
	/// モデルデータをワイヤーフレームとして描画リストに追加
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="world">座標情報</param>
	static void AddWireFlameData(Model& model, WorldTransform& world);
	/// <summary>
	/// スプライトを描画リストに追加
	/// </summary>
	/// <param name="sprite">スプライトデータ</param>
	/// <param name="world">座標情報</param>
	static void AddSpriteData(Sprite& sprite, WorldTransform& world);
	/// <summary>
	/// パーティクルを描画リストに追加
	/// </summary>
	/// <param name="particle">パーティクルデータ</param>
	static void AddParticleData(ParticleSystem& particle);
	 /// <summary>
	 /// パイプラインを変更
	 /// </summary>
	 /// <param name="Type"></param>
	 void ChangePipeline(PipelineType Type);

	 static void SetViewProj(CameraParameter parameter) {
		 viewProjection.parameter = parameter;
	 };
	 static ViewProjection& GetViewProjection() { return viewProjection; }
private:
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer& obj) = delete;
	Renderer& operator=(const Renderer& obj) = delete;

	static ViewProjection viewProjection;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList = nullptr;
	std::unique_ptr<PSOManager> PSOManager_ = nullptr;

	static std::vector<DrawModelData> drawModelData_;
	static std::vector<DrawSkinningData> drawModelSkinningData_;
	static std::vector<DrawSkinningDissolveData> drawModelSkinningDissolveData_;
	static std::vector<DrawLineData> drawLineData_;
	static std::vector<DrawModelData> drawWireFlameData_;
	static std::vector<DrawSpriteData> drawSpriteData_;
	static std::vector<DrawParticleData> drawParticleData_;

	SkyBox skyBox;

	int Texture;
	int dissolveTexture;
	WorldTransform cubeWorld_;
};
