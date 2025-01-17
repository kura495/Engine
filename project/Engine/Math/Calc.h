#pragma once
#include <cmath>
#include <numbers>
/// <summary>
/// 線形補間
/// </summary>
/// <param name="a">float</param>
/// <param name="b">float</param>
/// <param name="t">パラメーター</param>
/// <returns>float</returns>
float Lerp(float a, float b, float t);
/// <summary>
/// 最短
/// </summary>
/// <param name="a">float</param>
/// <param name="b">float</param>
/// <param name="t">パラメーター<</param>
/// <returns></returns>
float LerpShortAngle(float a,float b,float t);
