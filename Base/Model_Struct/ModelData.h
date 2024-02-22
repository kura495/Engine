#pragma once
#include "VertexData.h"
#include "Material.h"
#include <vector>

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
	int TextureIndex = 0;
};