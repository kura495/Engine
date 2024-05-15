#include"Line.hlsli"
struct TransformationMatrix
{
    float32_t4x4 matWorld;
    float32_t4x4 WorldInverseTranspose;
};
struct ViewProjectionMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 cameraPos;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);
struct VertexShaderInput
{
    float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float32_t4x4 WorldViewProjectionMatrix = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    output.position = mul(input.position, mul(gTransformationMatrix.matWorld, WorldViewProjectionMatrix));
    
    return output;
}