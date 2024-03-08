#include "Editer.h"
#include <CollisionManager.h>

Editer* Editer::GetInstance()
{
	static Editer instance;
	return &instance;
}

void Editer::Initalize()
{
	ObjectCount = 0;
	collisionManager = std::make_unique<CollisionManager>();

	boxModel_.push_back(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	planeModel_.push_back(Model::CreateModelFromObj("resources/Plane", "Plane.obj"));

	GlobalVariables::GetInstance()->AddItem("Editer", "BoxCount", boxObjectCount);
	boxObjectCount = GlobalVariables::GetInstance()->GetIntValue("Editer", "BoxCount");
	for (int32_t boxit = 0; boxit < boxObjectCount; boxit++) {
		AddBox();
	}
	GlobalVariables::GetInstance()->AddItem("Editer", "PlaneCount", PlaneObjectCount);
	PlaneObjectCount = GlobalVariables::GetInstance()->GetIntValue("Editer", "PlaneCount");
	for (int32_t Pleneit = 0; Pleneit < PlaneObjectCount; Pleneit++) {
		AddPlane();
	}
}

void Editer::Update()
{
	if (IsEnableFlag == false) {
		return;
	}
	if (viewProjection_ == nullptr) {
		return;
	}
	GuizmoOption();

	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		(*ObjectIt)->Update();
	}
	for (std::list<IObject*>::iterator ObjectIt = object_.begin(); ObjectIt != object_.end(); ObjectIt++) {
		collisionManager->AddBoxCollider((*ObjectIt));
	}

}

void Editer::Draw()
{
	if (IsEnableFlag == false) {
		return;
	}
	if (world_.empty()) {
		return;
	}
	if (viewProjection_ == nullptr) {
		return;
	}
	if (IsManipulatorFlag) {
		Manipulator();
	}
	if (IsGridFlag) {
		Grid();
	}
}

void Editer::GuizmoOption()
{
#ifdef USE_IMGUI
	ImGui::Begin("Editer");
	ImGui::InputInt("ObjectNumber", &ObjectCount);
	if (ObjectCount < 0) {
		ObjectCount = 0;
	}
	if (ObjectCount >= (int)world_.size()) {
		ObjectCount = (int)world_.size() - 1;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_G))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	ImGui::Checkbox("Manipulator", &IsManipulatorFlag);
	ImGui::Checkbox("DrawGrid", &IsGridFlag);
	ImGui::End();

	ImGui::Begin("Object");
	ImGui::DragFloat3("Scale", &world_[ObjectCount]->transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_[ObjectCount]->transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_[ObjectCount]->transform_.translate.x);
	world_[ObjectCount]->UpdateMatrix();
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

#endif
}

void Editer::Manipulator()
{
#ifdef USE_IMGUI
	Matrix4x4 GizmoMoveMatrix = world_[ObjectCount]->matWorld_;
	ImGuizmo::Manipulate(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &GizmoMoveMatrix.m[0][0]);
	
	Vector3 scale, rotate, translate;
	ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0], &translate.x, &rotate.x, &scale.x);
	world_[ObjectCount]->transform_.scale = scale;
	world_[ObjectCount]->transform_.translate = translate;
#endif
}

void Editer::Grid()
{
	Matrix4x4 IdentityMat = CreateIdentity4x4();
	ImGuizmo::DrawGrid(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
}

void Editer::ImGuimenu()
{
#ifdef USE_IMGUI
	

	ImGui::Begin("CreateObject", nullptr, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Box")) {

			if (ImGui::Button("Add Box")) {
				AddBox();
				boxObjectCount++;
				GlobalVariables::GetInstance()->Updateint32_tItem("Editer", "BoxCount", boxObjectCount);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Plane")) {

			if (ImGui::Button("Add Plane")) {
				AddPlane();
				PlaneObjectCount++;
				GlobalVariables::GetInstance()->Updateint32_tItem("Editer", "PlaneCount", PlaneObjectCount);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
#endif
}

void Editer::AddBox()
{
	BoxObject* box = new BoxObject;
	box->Initalize(boxModel_);

	std::string Number = std::to_string(box->GetNumber());

	std::string Name = "Box" + Number;
	GlobalVariables::GetInstance()->AddItem("Editer", Name, box->GetWorld().transform_);

	box->SetTransform(GlobalVariables::GetInstance()->GetTransformQuaValue("Editer", Name));

	boxObject_.push_back(box);
}
void Editer::AddPlane()
{
	PlaneObject* plane = new PlaneObject;
	plane->Initalize(planeModel_);

	std::string Number = std::to_string(plane->GetNumber());

	std::string Name = "Plane" + Number;
	GlobalVariables::GetInstance()->AddItem("Editer", Name, plane->GetWorld().transform_);

	plane->SetTransform(GlobalVariables::GetInstance()->GetTransformQuaValue("Editer", Name));

	planeObject_.push_back(plane);
}