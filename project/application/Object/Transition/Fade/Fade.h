#pragma once
//画面遷移クラス//
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Sprite/Sprite.h"
class Fade
{
public:	
	//初期化
	void Init();
	//フェードインをする時の初期化
	void InInit();
	//フェードアウトをする時の初期化
	void OutInit();
	//フェードイン用更新処理　終了するとtrueを返す
	bool In();
	//フェードアウト用更新処理　終了するとtrueを返す
	bool Out();
	void Draw();

private:

	std::unique_ptr<Sprite> sprite_;
	WorldTransform world_;

	float alpha_ = 0.0f;

};