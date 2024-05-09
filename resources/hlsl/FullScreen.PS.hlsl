#include "FullScreen.hlsli"


struct Material
{
    float32_t3 color; //å≥ÇÃêF
};
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    // Grayscale
    output.color = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 value = dot(output.color.rgb, gMaterial.color);
    output.color.rgb = value;
    
    return output;
}