#include "Calc.h"

float Lerp(float a,float b, float t) {
	float result;

	result = ((1 - t) * a) + ( b * t);

	return result;
}
float LerpShortAngle(float a, float b, float t)
{
	//角度差分を求める
	float diff = b - a;
	//diffを2*piで割った余りを求める
	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	//角度が-360より小さいなら
	if (diff < 2 * -(float)std::numbers::pi) {
		diff += 2 * (float)std::numbers::pi;
	}
	//角度が360より大きいなら
	else if (diff >= 2 * (float)std::numbers::pi) {
		diff -= 2 * (float)std::numbers::pi;
	}
	//diffを2*piで割った余りを求める
	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	//角度が-180より小さいなら
	if (diff < -(float)std::numbers::pi) {
		diff += 2 * (float)std::numbers::pi;
	}
	//角度が180より大きいなら
	else if (diff >= (float)std::numbers::pi) {
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a,diff,t);
}
