#pragma once
#include <string>
#include "Quaternion/Quaternion.h"
#include "Animation/Animation.h"
#include "Utility/GlobalTime.h"
#include "ParticleSystem/ParticleSystem.h"
class Player;
class ICollider;
enum class PlayerState {
	kRoot,
	kJump,
	kAttack,
	kDead,
	End
};
class IPlayerState
{
public:
	IPlayerState() { stateType = PlayerState::End; };
	virtual ~IPlayerState() {};

	virtual void Init(Player* p) = 0;
	virtual void Update(Player* p) = 0;
	virtual void Draw(Player* p) = 0;
	virtual void OnCollision(Player* p, const ICollider& collider) { p; collider; };
	virtual void OnCollisionAttack(Player* p, const ICollider& collider) { p; collider; };

	PlayerState GetStateType() { return stateType; };
	virtual std::string ShowState() { return "default"; };

protected:
	PlayerState stateType;
	float animationTime_ = 0.0f;

};