#pragma once

enum class PipelineType {
	Standerd,
	Skinning,
	SkinningDissolve,
	Particle,
	Sprite,
	DrawLine,
	WireFlame,
	CubeMap
};
enum class PostProsessType {
	PostProsessPSO,
	GaussianFilter,
	GrayScale,
	Vinette,
	LuminanceBasedOutline,
	RGBtoHSV,
};