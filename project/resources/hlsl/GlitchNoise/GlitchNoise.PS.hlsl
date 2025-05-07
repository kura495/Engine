struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
};
struct Material
{
    float32_t4 color; //元の色
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
    if (gMaterial.color.a == 0)
    {
        output.color = gTexture.Sample(gSampler, input.texcoord);
    }
    else
    {
        output.color = 0.0f;
        float32_t2 aaaaa = input.texcoord;
        aaaaa.x = gMaterial.color.r + input.texcoord.x;
        output.color.r = gTexture.Sample(gSampler, aaaaa).r;
        aaaaa.x = gMaterial.color.g + input.texcoord.x;
        output.color.g = gTexture.Sample(gSampler, aaaaa).g;
        aaaaa.x = gMaterial.color.b + input.texcoord.x;
        output.color.b = gTexture.Sample(gSampler, aaaaa).b;
    }

    
    return output;
}