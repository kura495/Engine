#include "WorkLockAt.h"

void WorkLockAt::Init(const Vector3& axis, const WorldTransform& target)
{
	Vector3 lockVector = { 0.0f,0.0f,0.0f };
	easeT = 0.0f;
	flag = true;

	lockVector = target.transform.translate - axis;
	lockVector = lockVector.Normalize();
	if (lockVector.z != 0.0) {
		//asinでsinから角度を計算
		lockAtRat = std::asin(lockVector.x / std::sqrt(lockVector.x * lockVector.x + lockVector.z * lockVector.z));
		//zが手前に向いている
		if (lockVector.z < 0.0) {
			//xが0より大きいかどうか
			//大きい = 円の右下
			//小さい = 円の左下
			lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> -lockAtRat : -std::numbers::pi_v<float> -lockAtRat;
		}
	}
	//zが真上を向いている場合
	else {
		lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> / 2.0f : -std::numbers::pi_v<float> / 2.0f;
	}
}

void WorkLockAt::Update(float& cameraRatVector)
{
	if (flag) {
		easeT = (std::min)(easeT + AddEaseT, 1.0f);
		cameraRatVector = LerpShortAngle(cameraRatVector, cameraRatVector + lockAtRat, easeT);
		if (easeT == 1.0f) {
			flag = false;
		}
	}
}
