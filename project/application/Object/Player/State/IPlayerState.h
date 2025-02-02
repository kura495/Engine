#pragma once
#include <string>
#include "Quaternion/Quaternion.h"
#include "Animation/Animation.h"
#include "Utility/GlobalTime.h"
class Player;
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
	PlayerState GetStateType() { return stateType; };
	virtual std::string ShowState() { return "default"; };
	PlayerState stateType;
protected:
	float animationTime_ = 0.0f;

};