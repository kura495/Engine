struct Vertex{
    float32_t4 position;
    float32_t2 texcoord;
    float32_t3 normal;
};
struct VertexInfluence{
    float32_t4 weight;
    int32_t4 index;
};
struct SkinningInformation{
    uint32_t numVerteces;
};
struct Well{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
};