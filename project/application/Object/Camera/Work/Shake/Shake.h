#pragma once
#include "../InterFace/Iwork.h"
#include "Vector/Vector2.h"
#include "Utility/random/random.h"
using namespace Math;


class Shake : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// 揺らす時間と大きさを設定
	/// </summary>
	/// <param name="Time">有効時間</param>
	/// <param name="ShakeValue">揺れの大きさ x = 最小値 y = 最大値</param>
	void Init(float Time, Vector2 ShakeValue) {
		easeT = 0.0f;
		flag = true;
		timer = Time;
		minShakeValue = ShakeValue.x;
		maxShakeValue = ShakeValue.y;
	}
	/// <summary>
	/// 揺らす処理
	/// </summary>
	/// <param name="translate">揺らす物の座標</param>
	void Update(Vector3& translate) {
		if (flag) {
			easeT = (std::min)(easeT + kDeltaTime, timer);

			Vector3 ramdomTranslate = { random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue, maxShakeValue) };

			translate += ramdomTranslate;

			if (easeT == timer) {
				flag = false;
			}
		}
	}
private:
	//ランダム最低値
	float minShakeValue = -0.7f;
	//ランダム最高値
	float maxShakeValue = 0.7f;
	//止める時間
	float timer = 0.0f;
};