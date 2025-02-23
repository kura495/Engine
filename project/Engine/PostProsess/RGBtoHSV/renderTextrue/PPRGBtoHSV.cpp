#include "PPRGBtoHSV.h"

void PPRGBtoHSV::addDraw()
{
	DirectX->GetcommandList()->SetGraphicsRootConstantBufferView(2, HSVResource.Get()->GetGPUVirtualAddress());
}

void PPRGBtoHSV::Update()
{
#ifdef _DEBUG
	ImGui::Begin("HSV");
	ImGui::DragFloat("hue",&hsvMaterial->hue,0.01f,-1.0f,1.0f);
	ImGui::DragFloat("saturation",&hsvMaterial->saturation, 0.01f, -1.0f, 1.0f);
	ImGui::DragFloat("value",&hsvMaterial->value, 0.01f, -1.0f, 1.0f);
	ImGui::End();
#endif
}

void PPRGBtoHSV::CreateBuffer()
{
	HSVResource = DirectX->CreateBufferResource(sizeof(HSVmaterial));
	HSVResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&hsvMaterial));
}
