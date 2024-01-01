#include "PlaneObject.h"

void PlaneObject::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
}

void PlaneObject::Update()
{
	world_.UpdateMatrix();
}

void PlaneObject::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void PlaneObject::ImGui()
{
	ImGui::Begin("PlaneObject");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	ImGui::End();
}
