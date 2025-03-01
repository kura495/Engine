#pragma once
#include "../IPlayerState.h"
class PRoot : public IPlayerState
{
public:
	PRoot() {
		stateType = PlayerState::Root;
		walkanimation = new Animation();
		walkanimation = Animation::LoadAnimationFile("project/resources/Player", "player_walk.gltf");
		walkanimation->Init();
	};

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
private:
	Animation* walkanimation;

};

