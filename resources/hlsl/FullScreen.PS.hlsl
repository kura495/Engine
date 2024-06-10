#include "FullScreen.hlsli"


struct Material
{
    float32_t3 color; //元の色
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
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    if (gMaterial.color.a == 1.0f)
    {
         
    }
    
        return output;
}