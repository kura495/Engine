#include "PlaneObject.h"

void PlaneObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Init();
	world_.Update();
	collider.Init(&world_);
	collider.SetSize({ 5.0f,0.0f,5.0f });
	collider.SetcollitionAttribute(Collider::Tag::Floor);
	collider.SetcollisionMask(~Collider::Tag::Floor);
	collider.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	collider.colliderName = "Plane";
}

void PlaneObject::Update()
{
	if (isCrash) {
		CrashEffect();
	}
	world_.Update();
	ImGui::Begin("PlaneObject");
	ImGui::End();
}

void PlaneObject::Draw()
{
	for (Model* model : models_) {
		if (collider.IsUsing) {
			model->RendererDraw(world_);
		}
	}
}

void PlaneObject::ImGui()
{
	ImGui::Begin("PlaneObject");
	ImGui::Text("Objectnumber %d", objectNumber_);
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	ImGui::End();
	std::string Number = std::to_string(GetNumber());

	std::string Name = "Plane" + Number;
	GlobalVariables::GetInstance()->UpdateTransformQuaItem("Editer", Name, world_.transform);
}

void PlaneObject::OnCollision(const ICollider& Collider)
{
	if (Collider.GetcollitionAttribute() == (Collider::Tag::EnemyAttackFront | Collider::Tag::EnemyAttackSlam)) {
		isCrash = true;
		return;
	}
}

void PlaneObject::CrashEffect()
{
	easeT = (std::min)(easeT + addEaseT,1.0f);
	world_.transform.translate.y -= 0.02f;
	//前のフレームのランダム加算を打ち消す
	world_.transform.translate += saveramdomTranslate;

	Vector3 ramdomTranslate = { random::Generate<float>(-0.1f, 0.1f),0.0f ,random::Generate<float>(-0.1f, 0.1f) };
	world_.transform.translate += ramdomTranslate;
	saveramdomTranslate = ramdomTranslate;
		ImGui::Begin("PlaneObject");
		ImGui::Text("Crash");
		ImGui::End();
	if (easeT == 1.0f) {
		easeT = 0.0f;
		isCrash = false;
		collider.IsUsing = false;
	}
}
