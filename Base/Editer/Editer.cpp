#include "Editer.h"

void Editer::Initalize()
{
}

void Editer::Update()
{
	GuizmoOption();
}

void Editer::Draw()
{
}

void Editer::GuizmoOption()
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	Matrix4x4 IdentityMat = CreateIdentity4x4();
	ImGuizmo::DrawGrid(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
#endif
}

void Editer::Manipulator()
{
	Matrix4x4 GizmoMoveMatrix = CreateIdentity4x4();
	GizmoMoveMatrix = world_[ObjectCount]->matWorld_;
	ImGuizmo::Manipulate(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD, &GizmoMoveMatrix.m[0][0]);
	
	float scale[3], rotate[3], translate[3];
	ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0], translate, rotate, scale);
	world_[ObjectCount]->transform_.scale.x = scale[0];
	world_[ObjectCount]->transform_.scale.y = scale[1];
	world_[ObjectCount]->transform_.scale.z = scale[2];
	world_[ObjectCount]->transform_.translate.x = translate[0];
	world_[ObjectCount]->transform_.translate.y = translate[1];
	world_[ObjectCount]->transform_.translate.z = translate[2];
}
