#pragma once
#include <random>
#include "Math/Math_Structs.h"
class random
{
public:
	/// <summary>
	/// 数字をランダムに出力する関数　int,floatのみ対応
	/// </summary>
	/// <typeparam name="T">intもしくはfloat</typeparam>
	/// <param name="Min">最小値</param>
	/// <param name="Max">最大値</param>
	/// <returns></returns>
	template <typename T>
	static T Generate(T Min, T Max)
	{
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> distribution(Min, Max);
			return distribution(randomEngine);
		}
		if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> distribution(Min, Max);
			return distribution(randomEngine);
		}
	}
private:

};
