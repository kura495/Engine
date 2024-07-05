#include "FullScreen.hlsli"
static const uint32_t kNumVertex = 3;
static const float32_t4 kPositions[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, // ¶ã
    { 3.0f, 1.0f, 0.0f, 1.0f }, // ‰Eã
    { -1.0f, -3.0f, 0.0f, 1.0f }, // ¶‰º
};
static const float32_t2 kTexcoords[kNumVertex] =
{
    { 0.0f, 0.0f }, // ¶ã
    { 2.0f, 0.0f }, // ‰Eã
    { 0.0f, 2.0f } // ¶‰º
};

VertexShaderOutput main(uint32_t vertexId : SV_VertexID)
{
    VertexShaderOutput output;
    output.position = kPositions[vertexId];
    output.texcoord = kTexcoords[vertexId];
    return output;
}
