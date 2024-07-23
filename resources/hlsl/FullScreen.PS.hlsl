#include "FullScreen.hlsli"

struct Material
{
    float32_t3 color; //元の色
};
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f , -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f ,  0.0f }, { 0.0f,  0.0f }, { 1.0f,  0.0f } },
    { { -1.0f ,  1.0f }, { 0.0f,  1.0f }, { 1.0f,  1.0f } },
};

static const float32_t kKernel3x3[3][3] =
{
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
        return output;
}