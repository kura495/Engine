#pragma once
//敵キャラクター基底クラス//
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"

class Player;

class Enemy : public IObject
{
public:

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(Player* player) { player_ = player; };

	bool GetIsAlive() {
		return IsAlive;
	}
	/// <summary>
	/// スケール
	/// 回転
	/// 移動
	/// </summary>
	void ImGui();
protected://関数
	//追加用ImGui overrideするだけで追加可能
	virtual void AddImGui(){};
	//当たり判定用
	virtual void OnCollision(const ICollider* ICollider);

	float animationTime_ = 0.0f;

	bool isDamege = false;
	int damegeInterval = 0;
	const int kDamegeInterval = 60;

	uint32_t HP_;

	bool IsAlive = true;

	Player* player_ = nullptr;
	//識別用の名前　変えることでImGuiのタブネームを変えることができる
	std::string name = "Enemy";
};
