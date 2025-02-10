#include "PJump.h"
#include "../../Player.h"
void PJump::Init(Player* p)
{
	p;
	jumpForce = kJumpForce;
}

void PJump::Update(Player* p)
{
	//移動関数
	p->Move();
	//ジャンプの処理
	p->GetWorld().transform.translate.y += jumpForce;
	jumpForce -= kJumpSubValue;
}

void PJump::Draw(Player* p)
{
	p->Getmodels()[Player::PlayerModel::MainBody]->RendererDraw(p->GetWorld());
}

void PJump::OnCollision(Player* p, const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::Floor) {
		p->GetWorld().transform.translate.y = collider.GetCenter().y;
		p->GetWorld().Update();
			if (p->GetWorld().transform.translate.y <= collider.GetCenter().y && jumpForce <= 0) {
				p->GetWorld().transform.translate.y = collider.GetCenter().y;
				p->GetWorld().Update();
				p->ChangeState<PRoot>();
			}

	}
}

std::string PJump::ShowState()
{
	return "Jump";
}
