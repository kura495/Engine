#pragma once

enum class PipelineType {
	Model,
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