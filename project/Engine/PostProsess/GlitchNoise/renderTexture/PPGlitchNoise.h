#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class PPGlitchNoise : public PostProsess
{
public:
	void Update()override;
	static bool isEnableFlag;
private:
	void Reset();

	//現在のTの値
	float easeT = 0.04f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.0005f;

	const float kEaseTMax = 0.08f;
	const float kEaseTMin = 0.005f;
};

