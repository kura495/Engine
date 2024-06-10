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
    //// Vignette
    //// 周囲を０に、中心になるほど明るくなるように計算で調整
    //float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    //// correctだけで計算すると中心の最大値が0.0625で暗すぎるのでScaleで調整。この例では16倍している
    //float vinette = correct.x * correct.y * 16.0f;
    //// とりあえず0.8乗でそれっぽくしてみた
    //vinette = saturate(pow(vinette, gMaterial.color.r));
    ////係数として乗算
    //output.color.rgb *= vinette;
    
    return output;
}