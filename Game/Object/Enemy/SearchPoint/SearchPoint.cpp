#include "SearchPoint.h"

void SearchPoint::Initalize(WorldTransform* Parent,Vector3 HitBoxSize)
{
	BoxCollider::Initialize();
	Collider::SetSize(HitBoxSize);
	Collider::SetWorld(Parent);
	SetcollitionAttribute(kCollitionAttributeSearchPoint);
	SetcollisionMask(~kCollitionAttributeSearchPoint && ~kCollitionAttributeEnemy);
}

void SearchPoint::Update()
{
	IsSearchOn = false;
	BoxCollider::Update();
}

void SearchPoint::Draw()
{

}

void SearchPoint::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributePlayer) {
#ifdef _DEBUG
		ImGui::Begin("SearchPointHitPlayer");

		ImGui::End();
#endif
		IsSearchOn = true;
	}
}
