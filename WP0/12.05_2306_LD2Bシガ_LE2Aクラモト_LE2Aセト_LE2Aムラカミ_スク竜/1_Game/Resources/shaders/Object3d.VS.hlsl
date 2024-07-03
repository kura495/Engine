#include"Object3d.hlsli"

struct WorldTransform {
	float32_t4x4 world;
	float32_t4x4 worldInverseTranspose;
};
ConstantBuffer<WorldTransform> gWorldTransform : register(b1);

struct Camera {
	float32_t4x4 view;
	float32_t4x4 projection;
	float32_t3 cameraPos;
};
ConstantBuffer<Camera> gCamera : register(b2);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, mul(gWorldTransform.world, mul(gCamera.view, gCamera.projection)));
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gWorldTransform.worldInverseTranspose));
	output.worldPosition = mul(input.position, gWorldTransform.world).xyz;
	return output;
}