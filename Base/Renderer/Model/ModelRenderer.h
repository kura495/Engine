#pragma once
#include "Base/Object/Model/Model.h"

class ModelRenderer
{
public:
	ModelRenderer();
	~ModelRenderer();

	void Init();
	void Update();
	void Draw();

private:
	void AddDrawData(const Model* modelData);
	std::vector<Model*>DrawFata;
};

ModelRenderer::ModelRenderer()
{
}

ModelRenderer::~ModelRenderer()
{
}

