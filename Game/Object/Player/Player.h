#pragma once

#include "Base/Object/IObject.h"
#include "Game/Object/Weapon/Weapon.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

enum class Behavior {
	kRoot,
	kAttack,
	kStep
};

class Player : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider);

	void Move();
	void PlayerRoring();
	static bool playerMoveValue;
	static bool PushOptionButtern;

#pragma region Getter
	WorldTransform& GetWorldTransform() {
		return world_;
	}
#pragma endregion

private:
	Input* input = nullptr;

#pragma region 
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//kRoot
	void RootInit();
	void RootUpdate();
	//kAttack
	void AttackInit();
	void AttackUpdate();
	//kStep
	void StepInit();
	void StepUpdate();
#pragma endregion BehaviorTree

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Vector3 lookPoint;
	Vector3 sub;

	Vector3 move;
	//プレイヤーの移動
	float speed = 0.1f;

	Vector3 tlanslatePre = { 0.0f,0.0f,0.0f };

	std::unique_ptr<Weapon> weapon_;

	float attack = 0.0f;

	Animation animation;

	OBBoxCollider collider;

	//Step関係
	//プレイヤーが前転をしながら
	void Rolling();
	//プレイヤーが後ろ側に回避
	void BackStep();
	bool IsEndStep = false;
	//移動の量
	const float kStepValue = 3.0f;


};