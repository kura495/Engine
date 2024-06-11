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
    
    if (gMaterial.color.r == 1.0f)
    {
        uint32_t width, height;
        gTexture.GetDimensions(width,height);
        float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
        output.color.rgb = float32_t3(0.0f,0.0f,0.0f);
        output.color.a = 1.0f;
        
        for (uint32_t x = 0; x < 3; ++x)
        {
            for (uint32_t y = 0; y < 3; ++y)
            {
                float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
                float32_t3 fetchColor = gTexture.Sample(gSampler,texcoord).rgb;
                output.color.rgb += fetchColor * kKernel3x3[x][y];
            }

        }
        
    }
    
        return output;
}