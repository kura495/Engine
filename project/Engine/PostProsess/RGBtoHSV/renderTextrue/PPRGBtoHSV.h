#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

struct HSVmaterial {
	float hue;
	float saturation;
	float value;
};

class PPRGBtoHSV : public PostProsess
{
public:
	void addDraw()override;
	void Update()override;
	void CreateBuffer()override;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> HSVResource = nullptr;
	HSVmaterial* hsvMaterial = nullptr;
};