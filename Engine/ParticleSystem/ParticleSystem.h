#pragma once
#include <random>
#include <numbers>

#include "Common/DirectX/DirectXCommon.h"
#include "Manager/SRV/SRVManager.h"
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Math/Matrix/MatrixCalc.h"
#include "ModelData.h"
#include "Light/Light.h"
#include "Transform.h"
#include "Utility/ImGuiManager.h"
#include "Collider/Box/BoxCollider.h"
#include "PipeLine/ParticlePipeLine.h"
#include "Emitter/Emitter.h"
/*パーティクルシステムクラス*/
static const float kDeltaTime = 1.0f / 60.0f;

struct Particle {
	Matrix4x4 matWorld;
	Vector3 velocity;
	Vector4 color;
	//生成されてから消えるまでの時間
	float lifeTime;
	//生成されてからの経過時間
	float currentTime;
	Transform transform;
};
struct ParticleForGPU {
	Matrix4x4 matWorld;
	Vector4 color;
};
struct AccelerationField {
	Vector3 acceleration;//加速度
	AABBData area;			 //範囲
};

class ParticleSystem
{
public:

	void Initalize(const std::string filePath);

	void Update();

	//関数ポインタ　UpdateParticleを入れる
	using UpdateFunction = std::function<void(Particle&)>;
	UpdateFunction UpdateParticle;
	//関数ポインタ　CustumSpawnを入れる
	using CustumSpawnFunction = std::function<Particle()>;
	/// <summary>
	/// カスタムスポーン　生成関数
	/// </summary>
	CustumSpawnFunction CustumSpawn;

	void RendererDraw();
	void Draw(const ViewProjection& viewProjection);

	void SpawnParticle(Emitter& emitter, std::mt19937& randomEngine);
	/// <summary>
	/// カスタムスポーンを使ってパーティクルを生成
	/// </summary>
	/// <param name="emitter"></param>
	void CustumSpawnParticle(Emitter& emitter);
	//ランダム
	Particle MakeNewParticle(Emitter& emitter, std::mt19937& randomEngine);
	/// <summary>
	/// パーティクルの位置をランダムに設定
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	Vector3 SetParticlePos(std::mt19937& randomEngine);
	/// <summary>
	/// パーティクルの色をランダムに設定
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	Vector4 SetParticleColor(std::mt19937& randomEngine);
	/// <summary>
	/// パーティクルの生存時間をランダムに設定
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <returns></returns>
	static float SetParticleLifeTime(std::mt19937& randomEngine, float min, float max);
private:
	std::list<Particle> Emit(Emitter& emitter, std::mt19937& randomEngine);
	std::list<Particle> CustumEmit(Emitter& emitter);
	
	bool IsCollision(const AABBData& aabb, const Vector3& point);

	void ImGui();

	//インスタンスの数
	static const uint32_t kNumMaxInstance = 100;
	//生きているインスタンスの数
	uint32_t numInstance = 0;

	ModelData modelData;

	DESCRIPTERHANDLE textureSrvHandle;

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	SRVManager* sRVManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource = nullptr;
	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	Material* materialData = nullptr;
	//Instancing用にTransformMatrixを複数格納できるResourcesを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> InstancingResource = nullptr;

	std::list<Particle> particles;
	ParticleForGPU* instancinsData;

	void CreateResources();
	void CreateSRV();


	Emitter Testemitter;

	AccelerationField TestField;
};

