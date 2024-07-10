#pragma once

#include "Game/Object/Character.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/ParticleSystem/ParticleSystem.h"

class Weapon:public OBBoxCollider
{
public:
	void Initalize(std::vector<Model*> models);
	void Update();
	void Draw();

	void ImGui();

	void OnCollision(const ICollider* collider);

	void SetParent(const WorldTransform& parent);

	void RootInit();
	void RootUpdate();

	void AttackInit();
	void AttackUpdate();

	void DrawParticle();

	void SetSize(Vector3 pos) {
		OBBoxCollider::SetSize({ pos.x + world_.transform_.translate.x,1.0f,pos.z + +world_.transform_.translate.z});
	}
	void SetPos(Vector3 vec) {
		world_.transform_.translate = vec;
		world_.UpdateMatrix();
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


	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	void addCameraRot();

	bool IsAttackOver = false;
};
