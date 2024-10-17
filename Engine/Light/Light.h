#pragma once
#include "Math_Structs.h"
#include "ImGuiManager.h"
#include <d3d12.h>
#include <wrl.h>

enum Lighting {
		NotDo = false,
		harfLambert = 1,
		Lambert = 2,
		phong = 3,
		EnvironmentMap = 4,
};

class Light
{
	struct DirectionalLight {
	Vector4 color;//ライトの色
	Vector3 direction;//ライトの向き
	float intensity;//ライトの輝度
};

public:
	static Light* GetInstance();
	void Initialize();
	void Update();
	ID3D12Resource* GetDirectionalLight()const {
		return directionalLightResource.Get(); }
	DirectionalLight& GetLightData() { return LightData; }
private:
	Light() = default;
	~Light() = default;
	Light(const Light& obj) = delete;
	Light& operator=(const Light& obj) = delete;

	DirectXCommon* directX_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource = nullptr;
	DirectionalLight* directionalLightData = nullptr;
	DirectionalLight LightData;
};

