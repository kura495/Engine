#pragma once
//画面遷移クラス//
#include "Texture/TextureManager.h"
#include "WorldTransform/WorldTransform.h"
#include "Sprite/Sprite.h"
#include "Utility/GlobalTime.h"
class Fade
{
public:	
	//フェードイン用初期化
	void InInit();
	//フェードイン用初期化
	void OutInit();
	/// <summary>
	/// フェードイン用更新処理　カウントが指定した秒数に達するとtrueを返す
	/// </summary>
	/// <param name="endFlame">秒数</param>
	/// <returns></returns>
	bool In(float second);
	/// <summary>
	/// フェードアウト用更新処理　カウントが指定した秒数に達するとtrueを返す
	/// </summary>
	/// <param name="endFlame">秒数</param>
	/// <returns></returns>
	bool Out(float second);

	void Draw();

private:
	//初期化
	void Init();

	std::unique_ptr<Sprite> sprite_;
	WorldTransform world_;

	float alpha_ = 0.0f;
	float timeCount_ = 0.0f;
};