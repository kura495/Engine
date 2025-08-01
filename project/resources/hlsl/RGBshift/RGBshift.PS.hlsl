struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
};
struct Material
{
    float32_t4 texcoord; //位置
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
    if (gMaterial.texcoord.a == 0)
    {
        output.color = gTexture.Sample(gSampler, input.texcoord);
    }
    else
    {
        output.color = 0.0f;
        float32_t2 newTexcoord = input.texcoord;
        newTexcoord.x = gMaterial.texcoord.x + input.texcoord.x;
        output.color.r = gTexture.Sample(gSampler, newTexcoord).r;
        newTexcoord.x = gMaterial.texcoord.y + input.texcoord.x;
        output.color.g = gTexture.Sample(gSampler, newTexcoord).g;
        newTexcoord.x = gMaterial.texcoord.z + input.texcoord.x;
        output.color.b = gTexture.Sample(gSampler, newTexcoord).b;
    }

    
        return output;
    }