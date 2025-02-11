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
	//有効でないなら
	if (IsEnableFlag == false) {
		return;
	}
	//オブジェクトがないなら
	if (object_.empty()) {
		return;
	}
	//ビュープロジェクションがないなら
	if (viewProjection_ == nullptr) {
		return;
	}
	GuizmoOption();
}

void Editer::Draw()
{
	//有効でないなら
	if (IsEnableFlag == false) {
		return;
	}
	//オブジェクトがないなら
	if (object_.empty()) {
		return;
	}
	//ビュープロジェクションがないなら
	if (viewProjection_ == nullptr) {
		return;
	}
	//マニピュレーターが有効なら
	if (IsManipulatorFlag) {
		Manipulator();
	}
	//グリッドが有効なら
	if (IsGridFlag) {
		Grid();
	}
}

void Editer::SetObject(IObject* object)
{
	object_.push_back(object);
}

void Editer::GuizmoOption()
{
#ifdef USE_IMGUI
#pragma region
	ImGui::Begin("Editer");
	//オブジェクトの選択
	if (ImGui::InputInt("ObjectNumber", &ObjectCount)) {
		if (ObjectCount < 0) {
			ObjectCount = 0;
		}
		if (ObjectCount >= (int)object_.size()) {
			ObjectCount = (int)object_.size() - 1;
		}
		textrueName = object_[ObjectCount]->Getmodels().at(0)->GetModelData().material.textureFilePath;
	};
	//移動、回転、拡縮を選択
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
	ImGui::DragFloat3("Scale", &object_[ObjectCount]->GetWorld().transform.scale.x);
	ImGui::DragFloat4("Rotate", &object_[ObjectCount]->GetWorld().transform.quaternion.x);
	ImGui::DragFloat3("Translate", &object_[ObjectCount]->GetWorld().transform.translate.x);
	
	ImGui::InputText("string", &textrueName, 256);
	//テクスチャを名前から変更
	if (ImGui::Button("Textrue")) {
		object_[ObjectCount]->Getmodels().at(0)->GetModelData().TextureIndex = TextureManager::GetInstance()->LoadTexture(textrueName);
	}
	//オブジェクトにドラッグアンドドロップしたテクスチャを適応
	if (!WinApp::dropFileName.empty()) {
		object_[ObjectCount]->Getmodels().at(0)->GetModelData().TextureIndex = TextureManager::GetInstance()->LoadTexture(WinApp::dropFileName);
		WinApp::dropFileName.clear();
	}

	object_[ObjectCount]->GetWorld().Update();
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

#endif
}

void Editer::Manipulator()
{
#ifdef USE_IMGUI
	Matrix4x4 GizmoMoveMatrix = object_[ObjectCount]->GetWorld().matWorld_;
	ImGuizmo::Manipulate(&Renderer::GetViewProjection().matView.m[0][0], &Renderer::GetViewProjection().matProjection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &GizmoMoveMatrix.m[0][0]);
	
	Vector3 scale, rotate, translate;
	ImGuizmo::DecomposeMatrixToComponents(&GizmoMoveMatrix.m[0][0], &translate.x, &rotate.x, &scale.x);
	object_[ObjectCount]->GetWorld().transform.scale = scale;
	object_[ObjectCount]->GetWorld().transform.translate = translate;
#endif
}

void Editer::Grid()
{
	Matrix4x4 IdentityMat = Matrix4x4::CreateIdentity();
	ImGuizmo::DrawGrid(&Renderer::GetViewProjection().matView.m[0][0], &Renderer::GetViewProjection().matProjection.m[0][0], &IdentityMat.m[0][0], 100.f);
}
