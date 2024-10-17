#include "../FullScreen.hlsli"
struct HSVMaterial
{
    float32_t hue;
    float32_t saturation;
    float32_t value;
};
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

static const float32_t kKernel3x3[3][3] =
{
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
};

HSVMaterial RGBtoHSV(float32_t3 rgb)
{
    HSVMaterial hsvMaterial;
    //最大値を調べる
    float32_t max = rgb.r > rgb.g ? rgb.r : rgb.g;
    max = max > rgb.b ? max : rgb.b;
    //最小値を調べる
    float32_t min = rgb.r < rgb.g ? rgb.r : rgb.g;
    min = min < rgb.b ? min : rgb.b;
    
    hsvMaterial.hue = max - min;
    if (hsvMaterial.hue > 0.0f)
    {
        if (max == rgb.r)
        {
            hsvMaterial.hue = (rgb.g - rgb.b) / hsvMaterial.hue;
            if (hsvMaterial.hue < 0.0f)
            {
                hsvMaterial.hue += 6.0f;
            }
        }
        else if (max == rgb.g)
        {
            hsvMaterial.hue = 2.0f + (rgb.b - rgb.r) / hsvMaterial.hue;
        }
        else
        {
            hsvMaterial.hue = 4.0f + (rgb.r - rgb.g) / hsvMaterial.hue;
        }
    }
    hsvMaterial.hue /= 6.0f;
    hsvMaterial.saturation = (max - min);
    if (max != 0.0f)
        hsvMaterial.saturation /= max;
    hsvMaterial.value = max;
    
    return hsvMaterial;
};
float32_t3 HSVtoRGB(HSVMaterial hsv)
{
    Material material;
    material.color.r = hsv.value;
    material.color.g = hsv.value;
    material.color.b = hsv.value;
    if (hsv.saturation > 0.0f)
    {
        hsv.hue *= 6.0f;
        uint32_t i = (uint32_t)hsv.hue;
        float32_t f = hsv.hue - (float32_t)i;
        switch (i)
        {
            default:
            case 0:
                material.color.g *= 1 - hsv.saturation * (1 - f);
                material.color.b *= 1 - hsv.saturation;
                break;
            case 1:
                material.color.r *= 1 - hsv.saturation * f;
                material.color.b *= 1 - hsv.saturation;
                break;
            case 2:
                material.color.r *= 1 - hsv.saturation;
                material.color.b *= 1 - hsv.saturation * (1 - f);
                break;
            case 3:
                material.color.r *= 1 - hsv.saturation;
                material.color.g *= 1 - hsv.saturation * f;
                break;
            case 4:
                material.color.r *= 1 - hsv.saturation * (1 - f);
                material.color.g *= 1 - hsv.saturation;
                break;
            case 5:
                material.color.g *= 1 - hsv.saturation;
                material.color.b *= 1 - hsv.saturation * f;
                break;
        }
    }
    
    return material.color;
};
float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange,range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<HSVMaterial> gHSVMaterial : register(b1);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
 
    HSVMaterial hsv = RGBtoHSV(textureColor.rgb);
    
    //
    hsv.hue += gHSVMaterial.hue;
    hsv.saturation += gHSVMaterial.saturation;
    hsv.value += gHSVMaterial.value;
    hsv.hue = WrapValue(hsv.hue, 0.0f, 1.0f);
    hsv.saturation = saturate(hsv.saturation);
    hsv.value = saturate(hsv.value);
    
    float32_t3 rgb = HSVtoRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    
        return output;
    }