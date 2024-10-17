#include "../FullScreen.hlsli"


struct Material
{
    float32_t3 color;
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
    if (gMaterial.color.r == 1.0f)
    {
        float32_t value = dot(output.color.rgb, float32_t3(0.2125f,0.7154f,0.0721f));
        output.color.rgb = float32_t3(value, value, value);
    }

    
    return output;
}
