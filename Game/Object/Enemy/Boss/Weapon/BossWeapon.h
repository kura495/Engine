#pragma once
#include "Game/Object/Character.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "ParticleSystem/ParticleSystem.h"

class BossWeapon
{
public:
	BossWeapon();
	~BossWeapon();

	void Initalize(std::vector<Model*> models);
	void Update();
	void UpdateMat(Matrix4x4 matrix);
	void Draw();

	void ImGui();

	void OnCollision(const ICollider* Icollider);

	void SetParent(const WorldTransform& parent);

	void RootInit();
	void RootUpdate();

	void AttackInit();
	void AttackUpdate();

	void DrawParticle();

	void SetSize(Vector3 pos) {
		collider.SetSize({ pos.x + world_.transform.translate.x,1.0f,pos.z + +world_.transform.translate.z });
	}
	void SetPos(Vector3 vec) {
		world_.transform.translate = vec;
		world_.Update();
	}

#pragma region Setter
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
#pragma endregion

#pragma region Getter
	bool GetIsAttackOver() const {
		return IsAttackOver;
	}
#pragma endregion

private:
	std::vector<Model*> models_;
	WorldTransform world_;

	Quaternion firstPos;
	Quaternion attackFirstQua;
	Quaternion attackEndQua;
	float AttackQuaParam_t = 0.0f;

	OBBoxCollider collider;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	void addCameraRot();

	bool IsAttackOver = false;
};
