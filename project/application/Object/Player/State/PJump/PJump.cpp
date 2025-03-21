#include "PJump.h"
#include "../../Player.h"
void PJump::Init(Player* player)
{
	player;
	jumpForce = kJumpForce;
}

void PJump::Update(Player* player)
{
	//移動関数
	player->Move();
	//ジャンプの処理
	player->GetWorld().transform.translate.y += jumpForce;
	jumpForce -= kJumpSubValue;
}

void PJump::Draw(Player* player)
{
	player->Getmodels()[Player::PlayerModel::MainBody]->RendererDraw(player->GetWorld());
}

void PJump::OnCollision(Player* player, const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::Floor) {
		player->GetWorld().transform.translate.y = collider.GetCenter().y;
		player->GetWorld().Update();
			if (player->GetWorld().transform.translate.y <= collider.GetCenter().y && jumpForce <= 0) {
				player->GetWorld().transform.translate.y = collider.GetCenter().y;
				player->GetWorld().Update();
				player->ChangeState<PRoot>();
			}

	}
}

std::string PJump::ShowState()
{
	return "Jump";
}
