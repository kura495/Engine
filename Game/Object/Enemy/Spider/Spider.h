#pragma once
#include "Game/Object/Enemy/Enemy.h"

class Spider : public Enemy {
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

private://関数



};