#pragma once
/*パーティクルシステムクラス*/
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
#include "GlobalTime.h"
#include "Utility/random/random.h"

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

	void Init(const std::string filePath);

	void Update(bool billboardFlag = true);
	//関数ポインタ　UpdateParticleを入れる
	using UpdateFunction = std::function<void(Particle&)>;
	UpdateFunction UpdateFunc;
	//関数ポインタ　CustumSpawnを入れる
	using CustumSpawnFunction = std::function<Particle()>;
	/// <summary>
	/// カスタムスポーン　生成関数
	/// </summary>
	CustumSpawnFunction CustumSpawnFunc;

	void RendererDraw();
	void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// パーティクルを周期ごとに生成して行く関数
	/// </summary>
	/// <param name="particle"></param>
	/// <param name="emitter"></param>
	static void ParticleSpawn(ParticleSystem& particle, Emitter& emitter);
	/// <summary>
	/// パーティクルを周期ごとに生成して行く関数
	/// CustumSpawnFuncに渡された関数を使って生成
	/// </summary>
	/// <param name="particle"></param>
	/// <param name="emitter"></param>
	static void ParticleCustumSpawn(ParticleSystem& particle, Emitter& emitter);
	/// <summary>
	/// パーティクルを生成
	/// </summary>
	/// <param name="emitter"></param>
	void SpawnParticle(Emitter& emitter);
	/// <summary>
	/// カスタムスポーンを使ってパーティクルを生成
	/// </summary>
	/// <param name="emitter"></param>
	void CustumSpawnParticle(Emitter& emitter);
	//ランダム
	Particle MakeNewParticle(Emitter& emitter);
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
	Vector4 SetParticleColor();
private:
	std::list<Particle> Emit(Emitter& emitter);
	std::list<Particle> CustumEmit(Emitter& emitter);
	
	bool IsCollision(const AABBData& aabb, const Vector3& point);

	void ImGui();

	//インスタンスの数
	const uint32_t kNumMaxInstance = 100;
	//生きているインスタンスの数
	uint32_t numInstance;

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

