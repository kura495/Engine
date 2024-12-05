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

struct Particle {
	Matrix4x4 matWorld;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
	Transform transform;
};
struct ParticleForGPU {
	Matrix4x4 matWorld;
	Vector4 color;
};
//struct Emitter {
//	Transform transform;//エミッタのTransform
//	uint32_t count;		//発生数
//	float frequency;	//発生頻度
//	float frequencyTime;//頻度用時刻
//};
struct AccelerationField {
	Vector3 acceleration;//加速度
	AABBData area;			 //範囲
};

class ParticleSystem
{
public:

	void Initalize(const std::string filePath);

	void Update();

	//関数ポインタ　OnCollisionを入れる
	using UpdateFunction = std::function<void(Particle&)>;
	UpdateFunction UpdateParticle;

	void RendererDraw();
	void Draw(const ViewProjection& viewProjection);

	void SpawnParticle(Emitter& emitter, std::mt19937& randomEngine);

	void PreDraw();

	std::list<Particle> Emit(Emitter& emitter, std::mt19937& randomEngine);

	bool IsCollision(const AABBData& aabb, const Vector3& point);

private:
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

	std::unique_ptr<ParticlePipeLine> Pipeline_;

	const float kDeltaTime = 1.0f / 60.0f;

	//ランダム
	Particle MakeNewParticle(Emitter& emitter,std::mt19937& randomEngine);
	Vector4 MakeParticleColor(std::mt19937& randomEngine);
	float MakeParticleLifeTime(std::mt19937& randomEngine);

	Emitter Testemitter;

	AccelerationField TestField;
};

