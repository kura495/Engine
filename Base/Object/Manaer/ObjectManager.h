#pragma once
#include "Object/IObject.h"
#include "Game/Object/Box/BoxObject.h"
#include "Game/Object/Plane/PlaneObject.h"

class ObjectManager
{
public:

	static ObjectManager* GetInstance();

	void Initalize();
	void Update();
	void Draw(const ViewProjection& viewProj);

	void LordFile(std::string fileName);

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	std::list<IObject*> object_;
	int32_t boxObjectCount;
	int32_t PlaneObjectCount;
	GlobalVariables* globalVariables;
	void AddBox();
	void AddPlane();
};