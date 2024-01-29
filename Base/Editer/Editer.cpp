#include "Editer.h"

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
	GuizmoOption();
}

void Editer::Draw()
{
	if (world_.empty()) {
		return;
	}
	Manipulator();
}

void Editer::GuizmoOption()
{
	if (viewProjection_ == nullptr) {
		return;
	}

#ifdef _DEBUG
	ImGui::Begin("Editer");
	ImGui::InputInt("ObjectNumber", &ObjectCount);
	if (ObjectCount >= (int)world_.size()) {
		ObjectCount = (int)world_.size() - 1;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_E))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	ImGui::End();

	ImGui::Begin("Object");

	ImGui::DragFloat3("Scale", &world_[ObjectCount]->transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_[ObjectCount]->transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_[ObjectCount]->transform_.translate.x);
	world_[ObjectCount]->UpdateMatrix();
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	Matrix4x4 IdentityMat = CreateIdentity4x4();
	ImGuizmo::DrawGrid(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);

#endif
}

void Editer::Manipulator()
{
#ifdef _DEBUG
	Matrix4x4 GizmoMoveMatrix = world_[ObjectCount]->matWorld_;
	ImGuizmo::Manipulate(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &GizmoMoveMatrix.m[0][0]);
	
	Vector3 scale, rotate, translate;
	ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0], &translate.x, &rotate.x, &scale.x);
	world_[ObjectCount]->transform_.scale = scale;
	world_[ObjectCount]->transform_.translate = translate;
#endif
}
