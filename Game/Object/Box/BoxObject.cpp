#include "BoxObject.h"

void BoxObject::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
}

void BoxObject::Update()
{
	world_.UpdateMatrix();
}

void BoxObject::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void BoxObject::ImGui()
{
	ImGui::Begin("Box");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale",&world_.scale_.x);
	ImGui::DragFloat3("Rotate",&world_.quaternion.x);
	ImGui::DragFloat3("Translate",&world_.translation_.x);
	ImGui::End();
}

