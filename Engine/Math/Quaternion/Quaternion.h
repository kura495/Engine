#pragma once
#include "Math/Vector/Vector3.h"
#include "DirectXMath.h"
#include <numbers>
namespace Math {
	class Quaternion final {
	public:
		float x;
		float y;
		float z;
		float w;
		//jsonにマクロを利用して構造体を登録
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Quaternion, x, y, z, w)
	public:
#pragma region Oparator
		Quaternion operator+(const Quaternion input)const {
			return Quaternion(this->x + input.x, this->y + input.y, this->z + input.z, this->w + input.w);
		}
		Quaternion operator+=(const Quaternion input) {
			*this = *this + input;
			return *this;
		}
		Quaternion operator-(const Quaternion input)const {
			return Quaternion(this->x - input.x, this->y - input.y, this->z - input.z, this->w - input.w);
		}
		Quaternion operator-=(const Quaternion input) {
			*this = *this - input;
			return *this;
		}
		Quaternion operator*(const Quaternion input)const {
			Quaternion result;
			Vector3 cross = Vector3::Cross({ this->x,this->y,this->z }, { input.x,input.y,input.z });
			float dot = Vector3::Dot({ this->x,this->y,this->z }, { input.x,input.y,input.z });
			result.x = cross.x + input.w * this->x + this->w * input.x;
			result.y = cross.y + input.w * this->y + this->w * input.y;
			result.z = cross.z + input.w * this->z + this->w * input.z;
			result.w = this->w * input.w - dot;

			return result;
		}
		Quaternion operator*=(const Quaternion input) {
			*this = *this * input;
			return *this;
		}
#pragma endregion
	public:
		/// <summary>
		/// 単位Quaternionを作る
		/// </summary>
		/// <returns>Quaternion</returns>
		static Quaternion IdentityQuaternion();
		/// <summary>
		/// 共役Quaternionを返す
		/// </summary>
		/// <param name="quaternion">クォータニオン</param>
		/// <returns>Quaternion</returns>
		static Quaternion Conjugate(const Quaternion& quaternion);
		/// <summary>
		/// 共役Quaternionを返す
		/// </summary>
		/// <returns>Quaternion</returns>
		Quaternion Conjugate();
		/// <summary>
		/// Quaternionのnormを返す
		/// </summary>
		/// <param name="quaternion">クォータニオン</param>
		/// <returns>float</returns>
		static float Norm(const Quaternion& quaternion);
		/// <summary>
		/// Quaternionのnormを返す
		/// </summary>
		/// <returns>float</returns>
		float Norm();
		/// <summary>
		/// 逆Quaternionを返す
		/// </summary>
		/// <param name="quaternion">クォータニオン</param>
		/// <returns>Quaternion</returns>
		static Quaternion Inverse(const Quaternion& quaternion);
		/// <summary>
		/// 逆Quaternionを返す
		/// </summary>
		/// <returns>Quaternion</returns>
		Quaternion Inverse();
		/// <summary>
		/// 正規化したQuaternionを返す
		/// </summary>
		/// <param name="quaternion">クォータニオン</param>
		/// <returns>Quaternion</returns>
		static Quaternion Normalize(const Quaternion& quaternion);
		/// <summary>
		/// 正規化したQuaternionを返す
		/// </summary>
		/// <returns>Quaternion</returns>
		Quaternion Normalize();
		/// <summary>
		/// 球面線形補間
		/// </summary>
		/// <param name="q0">クォータニオン</param>
		/// <param name="q1">クォータニオン</param>
		/// <param name="t">パラメーター</param>
		/// <returns>Quaternion</returns>
		static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
		/// <summary>
		/// オイラー角をクォータニオン
		/// </summary>
		/// <param name="input">オイラー角で表現された回転</param>
		/// <returns>Quaternion</returns>
		static Quaternion EulerToQuaterion(Vector3 input);
		/// <summary>
		/// 任意軸回転を表すQuaternionの生成
		/// </summary>
		/// <param name="axis">軸</param>
		/// <param name="angle">回転量</param>
		/// <returns>Quaternion</returns>
		static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
	};
}



