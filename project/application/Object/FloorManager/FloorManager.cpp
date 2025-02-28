#include "FloorManager.h"

void FloorManager::Init()
{
	planeModel_.push_back(Model::CreateModelFromObj("project/resources/Plane", "Plane.obj"));
	Create();
}

void FloorManager::Update()
{
	for (PlaneObject* obj : planeList) {
		obj->Update();
	}
}

void FloorManager::Draw()
{
	for (PlaneObject* obj : planeList) {
		obj->Draw();
	}
}

void FloorManager::ReStert()
{
	planeList.clear();
	Create();
}

void FloorManager::Create()
{
	float offset = (floorCount * 10.0f) / 2.0f;
	//縦
	for (int YIt = 0; YIt < floorCount; YIt++) {
		//横
		for (int XIt = 0; XIt < floorCount; XIt++) {
			PlaneObject* plane = new PlaneObject();
			plane->Init(planeModel_);
			plane->SetPos({ (10 * XIt) - offset + 5,0.0f,(10 * YIt) - offset + 5 });
			planeList.push_back(plane);
		}
	}
}
