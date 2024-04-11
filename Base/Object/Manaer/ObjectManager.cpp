#include "ObjectManager.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::Initalize()
{
	globalVariables = GlobalVariables::GetInstance();
}

void ObjectManager::Update()
{
#ifdef USE_IMGUI
	ImGui::Begin("CreateObject", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Box")) {

			if (ImGui::Button("Add Box")) {
				AddBox();
				boxObjectCount++;
				globalVariables->Updateint32_tItem("Editer", "BoxCount", boxObjectCount);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Plane")) {

			if (ImGui::Button("Add Plane")) {
				AddPlane();
				PlaneObjectCount++;
				globalVariables->Updateint32_tItem("Editer", "PlaneCount", PlaneObjectCount);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
#endif	//ImGui
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}

}

void ObjectManager::Draw(const ViewProjection& viewProj)
{
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Draw(viewProj);
	}
}

void ObjectManager::LordFile(std::string fileName)
{
	globalVariables->CreateGroup("Editer");
#pragma region
	globalVariables->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = globalVariables->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount; boxit++) {
		AddBox();
	}
	globalVariables->AddItem("Editer", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = globalVariables->GetIntValue("Editer", "PlaneCount");
	for (int32_t Pleneit = 0; Pleneit < PlaneObjectCount; Pleneit++) {
		AddPlane();
	}
#pragma endregion オブジェクト生成
}

void ObjectManager::AddBox()
{
	std::vector<Model*> boxModel_;
	boxModel_.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	BoxObject* box = new BoxObject;
	box->Initalize(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	globalVariables->AddItem("Editer", Name, box->GetWorld().transform_);

	box->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

	object_.push_back(box);
}

void ObjectManager::AddPlane()
{
	std::vector<Model*>planeModel_;
	planeModel_.push_back(Model::CreateModelFromObj("resources/Plane", "Plane.obj"));
	PlaneObject* plane = new PlaneObject;
	plane->Initalize(planeModel_);

	std::string Number = std::to_string(plane->GetNumber());

	std::string Name = "Plane" + Number;
	globalVariables->AddItem("Editer", Name, plane->GetWorld().transform_);

	plane->SetTransform(globalVariables->GetTransformQuaValue("Editer", Name));

	object_.push_back(plane);
}
