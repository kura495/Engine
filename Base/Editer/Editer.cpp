#include "Editer.h"

Editer* Editer::GetInstance()
{
	static Editer instance;
	return &instance;
}

void Editer::Initalize()
{
}

void Editer::Update()
{
	GuizmoOption();
	Manipulator();
}

void Editer::Draw()
{
}

void Editer::GuizmoOption()
{
#ifdef _DEBUG
	ImGui::Begin("Editer");


	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	Matrix4x4 IdentityMat = CreateIdentity4x4();
	ImGuizmo::DrawGrid(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
	ImGui::End();
#endif
}

void Editer::Manipulator()
{
#ifdef _DEBUG
	Matrix4x4 GizmoMoveMatrix = CreateIdentity4x4();
	GizmoMoveMatrix = world_[ObjectCount]->matWorld_;
	ImGuizmo::Manipulate(&viewProjection_->matView.m[0][0], &viewProjection_->matProjection.m[0][0], ImGuizmo::TRANSLATE, ImGuizmo::WORLD, &GizmoMoveMatrix.m[0][0]);
	
	Vector3 scale, rotate, translate;
	ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0], &translate.x, &rotate.x, &scale.x);
	world_[ObjectCount]->transform_.scale = scale;
	world_[ObjectCount]->transform_.translate = translate;
#endif
}
