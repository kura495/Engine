#pragma once
#include "Vector/Vector3.h"
#include "Calc.h"
#include "GlobalTime.h"

/// <summary>
/// work基底クラス
/// </summary>
class Iwork {
protected:
	//有効化フラグ
	bool flag = false;
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float AddEaseT = 0.02f;
};