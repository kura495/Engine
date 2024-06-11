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
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t PI = 3.1459265f;
float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    
    return exp(exponent) * rcp(denominator);
}

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    if (gMaterial.color.r == 1.0f)
    {
        float32_t weight = 0.0f;
        float32_t Kernel3x3[3][3];
        
        for (int32_t Ix = 0; Ix < 3; ++Ix)
        {
            for (int32_t Iy = 0; Iy < 3; ++Iy)
            {
                Kernel3x3[Ix][Iy] = gauss(kIndex3x3[Ix][Iy].x, kIndex3x3[Ix][Iy].y, 2.0f);
                weight += Kernel3x3[Ix][Iy];
            }
        }
        
        uint32_t width, height;
        gTexture.GetDimensions(width, height);
        float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
        output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
        output.color.a = 1.0f;
        
        for (int32_t x = 0; x < 3; ++x)
        {
            for (int32_t y = 0; y < 3; ++y)
            {
                float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
                float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
                output.color.rgb += fetchColor * Kernel3x3[x][y];

            }
        }
        output.color.rgb *= rcp(weight);
    }
    
    return output;
}