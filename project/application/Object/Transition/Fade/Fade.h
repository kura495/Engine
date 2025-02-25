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
	/// <summary>
	/// フェードイン用更新処理　カウントがendFlameに達するとtrueを返す
	/// 0から1へ向かう
	/// </summary>
	/// <param name="addFlame">1フレームごとの加算量</param>
	/// <param name="endFlame">秒数</param>
	/// <returns></returns>
	bool In(float addFlame,float endFlame);
	/// <summary>
	/// フェードアウト用更新処理　カウントがendFlameに達するとtrueを返す
	/// 1から0へ向かう
	/// </summary>
	/// <param name="addFlame">1フレームごとの加算量</param>
	/// <param name="endFlame">秒数</param>
	/// <returns></returns>
	bool Out(float addFlame, float endFlame);

	void Draw();

private:

	std::unique_ptr<Sprite> sprite_;
	WorldTransform world_;

	float alpha_ = 0.0f;
	float timeCount_ = 0.0f;
};