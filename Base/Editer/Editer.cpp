#include "Editer.h"
#include "Object/IObject.h"

Editer* Editer::GetInstance()
{
	static Editer instance;
	return &instance;
}

void Editer::Initalize()
{
	ObjectCount = 0;
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

void Editer::SetObject(IObject* object)
{
	object_.push_back(object);
	SetWorld(&object->GetWorld());
}

void Editer::GuizmoOption()
{
#ifdef USE_IMGUI
#pragma region

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
#pragma endregion

	ImGui::Begin("Object");
	ImGui::DragFloat3("Scale", &world_[ObjectCount]->transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_[ObjectCount]->transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_[ObjectCount]->transform_.translate.x);
	//const char* str = 
	//
	//if (ImGui::InputText("string",buf.c_str(), 256)) {
	//	ImGui::Text("InputText");
	//}
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
