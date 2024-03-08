#pragma once
#include <random>
#include <numbers>

#include "Common/DirectX/DirectXCommon.h"
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Math/Matrix/MatrixCalc.h"
#include "ModelData.h"
#include "Light/Light.h"
#include "Transform.h"
#include "Utility/ImGuiManager.h"
#include "Utility/BoxCollider.h"
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
	AABB area;			 //範囲
};

class ParticleSystem
{
public:

	void Initalize(const std::string filePath);

	void Update(const ViewProjection& viewProjection);

	void Draw(const ViewProjection& viewProjection);

	void PreDraw();

	void SetPos(Vector3 Pos);

	void AddParticle(const Emitter& emitter);

	std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);

	bool IsCollision(const AABB& aabb, const Vector3& point);

private:
	//インスタンスの数
	static const uint32_t kNumMaxInstance = 100;
	//生きているインスタンスの数
	uint32_t numInstance = 0;

	ModelData modelData;

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;

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

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSRVHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSRVHandleGPU;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	const float kDeltaTime = 1.0f / 60.0f;

	//ランダム
	Particle MakeNewParticle(std::mt19937& randomEngine,const Vector3& translate);
	Vector4 MakeParticleColor(std::mt19937& randomEngine);
	float MakeParticleLifeTime(std::mt19937& randomEngine);

	Emitter Testemitter;

	AccelerationField TestField;
};

