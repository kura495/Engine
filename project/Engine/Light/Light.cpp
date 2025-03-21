#include "Light.h"

Light* Light::GetInstance()
{
	static Light instance;
	return &instance;
}

void Light::Initialize()
{
	directX_ = DirectXCommon::GetInstance();

	directionalLightResource = directX_->CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	LightData.color = { 1.0f,1.0f,1.0f,1.0f };
	LightData.direction = { 0.0f,10.0f,0.0f };
	LightData.intensity = 1.0f;
	directionalLightData->color = LightData.color;
	directionalLightData->direction = LightData.direction;
	directionalLightData->intensity = LightData.intensity;
}

void Light::Update()
{
	directionalLightData->color = LightData.color;
	directionalLightData->direction = LightData.direction;
	directionalLightData->intensity = LightData.intensity;
}

