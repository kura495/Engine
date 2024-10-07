#include"../Object3d.hlsli"

struct Material
{
    float32_t4 color; //元の色
    int32_t enableLighting; //ライトのフラグ
    float32_t4x4 uvTransform; //uvのSRT
    float32_t shininess; //光沢度
};
struct DirectionalLight
{
    float32_t4 color; //ライトの色
    float32_t3 direction; //ライトの向き
    float intensity; //ライトの輝度
};
struct Camera
{
    float32_t3 worldPosition;
};
struct Value
{
    float32_t4 value;
};
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<Value> gValue : register(b3);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gTexture2 : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

 
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    output.color = gMaterial.color * textureColor;
	
    return output;
}