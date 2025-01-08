#pragma once
#include "Object/Manager/ObjectManager.h"
class FloorManager
{
public:
	void Init();
	void Update();
	void Draw();
private:
	//個数
	int floorCount = 5;
	std::vector<Model*> planeModel_;
	std::list<PlaneObject*>planeList;
};
