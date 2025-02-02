#include "PlaneObject.h"

void PlaneObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.Update();
	collider.Init(&world_);
	collider.SetSize({ 5.0f,0.0f,5.0f });
	collider.SetcollitionAttribute(Collider::Tag::Floor);
	collider.SetcollisionMask(~Collider::Tag::Floor);
	collider.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
}

void PlaneObject::Update()
{
	if (isCrash) {
		CrashEffect();
	}
	world_.Update();
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
	if (Collider.GetcollitionAttribute() == (Collider::Tag::EnemyAttackFront | Collider::Tag::EnemyAttack)) {
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
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-0.1f, 0.1f);
	Vector3 ramdomTranslate = { distribution(randomEngine),0.0f ,distribution(randomEngine) };
	world_.transform.translate += ramdomTranslate;
	saveramdomTranslate = ramdomTranslate;
	if (easeT == 1.0f) {
		isCrash = false;
		collider.IsUsing = false;
	}
}
