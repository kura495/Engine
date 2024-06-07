#include "Skinning.hlsli"

//Palette
StructuredBuffer<Well> gMatrixPalette : register(t0);
//Vertex
StructuredBuffer<Vertex> gInputVertices : register(t1);
//Influences
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
//SkinnedVertex
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
//SkinningInformation
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void main( uint32_t3 DTid : SV_DispatchThreadID)
{
    uint32_t vertexIndex = DTid.x;
    if (vertexIndex < gSkinningInformation.numVerteces) {
        //Skinning計算
        
        //StructuredBufferからデータを取ってくる
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        //Skinning後の頂点を計算
        #pragma region
        //位置の変換
        skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
        skinned.position.w = 1.0f; // 確実に1を入れる
        //UVの変換
        Vertex skinned;
        skinned.texcoord = input.texcoord;
	    //法線の変換
        skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
        skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
        skinned.normal = normalize(skinned.normal); // 正規化する
        
        //Skinning後の頂点データを書き込む
        gOutputVertices[vertexIndex] = skinned;
    }

}