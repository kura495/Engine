#include"Object3d.hlsli"

struct Material {
	float32_t4 color;//元の色
	int32_t enableLighting;//ライトのフラグ
	float32_t4x4 uvTransform;//uvのSRT
    float32_t shininess;//光沢度
};
struct DirectionalLight {
	float32_t4 color;//ライトの色
	float32_t3 direction;//ライトの向き
	float intensity;//ライトの輝度
};
struct Camera
{
    float32_t3 worldPosition;
};
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
Texture2D<float32_t4> gTexture : register(t0);
TextureCube<float32_t4> gEnvironmentTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

 
PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
    output.color = gMaterial.color * textureColor;
    
	//ライティング
    if (gMaterial.enableLighting != 0)
    { //Lightingする場合
        if (gMaterial.enableLighting == harfLambert)
        {
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            output.color.a = gMaterial.color.a * textureColor.a;
                        
            float32_t3 cameraPosition = normalize(input.worldPosition - gCamera.worldPosition);
            float32_t3 reflectedVector = reflect(cameraPosition, normalize(input.normal));
            float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
            //environmentColor.rgb *= 10;
            output.color.rgb += environmentColor.rgb;
            
        }
        if (gMaterial.enableLighting == phong)
        {
            float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
                //float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
            float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
            float RdotE = dot(reflectLight, toEye);
                //float NdotH = dot(normalize(input.normal), halfVector);
            float specularPow = pow(saturate(RdotE), gMaterial.shininess);
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
                //拡散反射
            float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
                //鏡面反射
            float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
                //拡散反射＋鏡面反射
            output.color.rgb = diffuse + specular;
                //アルファ
            output.color.a = gMaterial.color.a * textureColor.a;
            
            float32_t3 cameraPosition = normalize(input.worldPosition - gCamera.worldPosition);
            float32_t3 reflectedVector = reflect(cameraPosition, normalize(input.normal));
            float32_t4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectedVector);
            //environmentColor.rgb *= 10;
            output.color.rgb += environmentColor.rgb;
        }
    }
	
    return output;
}