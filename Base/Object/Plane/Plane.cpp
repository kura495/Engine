#include "Plane.h"

void Plane::Initalize(const std::string filePath)
{
	textureManager_ = TextureManager::GetInstance();
	directX_ = DirectXCommon::GetInstance();
	light_ = Light::GetInstance();

	modelData.vertices.push_back({ .position = { left.top },  .texcoord = {0.0f,0.0f},.normal = {0.0f,1.0f,0.0f} });//左上
	modelData.vertices.push_back({ .position = {right.top},	  .texcoord = {1.0f,0.0f},.normal = {0.0f,1.0f,0.0f} });//右上
	modelData.vertices.push_back({ .position = {left.bottom}, .texcoord = {0.0f,1.0f},.normal = {0.0f,1.0f,0.0f} });//左下
	modelData.vertices.push_back({ .position = {left.bottom}, .texcoord = {0.0f,1.0f},.normal = {0.0f,1.0f,0.0f} });//左下
	modelData.vertices.push_back({ .position = {right.top},	  .texcoord = {1.0f,0.0f},.normal = {0.0f,1.0f,0.0f} });//右上
	modelData.vertices.push_back({ .position = {right.bottom},.texcoord = {1.0f,1.0f},.normal = {0.0f,1.0f,0.0f} });//右下
	modelData.material.textureFilePath = filePath;
	int Texture = textureManager_->LoadTexture(modelData.material.textureFilePath);
	modelData.TextureIndex = Texture;
	
	world_.Initialize();

	CreateResources();

	materialData->enableLighting = Lighting::NotDo;
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->uvTransform = CreateIdentity4x4();
	
}

void Plane::Update()
{
#ifdef _DEBUG
	ImGui();
#endif
	world_.UpdateMatrix();
}

void Plane::Draw(const ViewProjection& viewProjection)
{

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//matWorld
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, world_.constBuff_->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData.TextureIndex));
	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Plane::CreateResources()
{
	//CreateVertexResource
	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//CreateVertexbufferView
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//maping vertexResource
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
	//CreatematerialResource
	materialResource = directX_->CreateBufferResource(sizeof(Material));
	//maping materialResource
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
}

void Plane::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Plane");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	ImGui::InputInt("Select", &selectNumber_,1,4);


	Vector4 Manipulate;
	if (selectNumber_ == 0) {
		Manipulate = modelData.vertices[0].position;
		ImGui::Text("LeftTop");
	}
	if (selectNumber_ == 1) {
		Manipulate = modelData.vertices[2].position;
		ImGui::Text("LeftBottom");
	}
	if (selectNumber_ == 2) {
		Manipulate = modelData.vertices[5].position;
		ImGui::Text("LeftBottom");
	}
	if (selectNumber_ == 3) {
		Manipulate = modelData.vertices[1].position;
		ImGui::Text("RightTop");
	}
	ImGui::DragFloat3("PointTranslate",&Manipulate.x);
	if (selectNumber_ == 0) {
		modelData.vertices[0].position = Manipulate;
		vertexData[0].position = Manipulate;
	}
	if (selectNumber_ == 1) {
		modelData.vertices[2].position = Manipulate;
		modelData.vertices[3].position = Manipulate;	
		vertexData[2].position = Manipulate;
		vertexData[3].position = Manipulate;
	}
	if (selectNumber_ == 2) {
		modelData.vertices[5].position = Manipulate;
		vertexData[5].position = Manipulate;
	}
	if (selectNumber_ == 3) {
		modelData.vertices[1].position = Manipulate;
		modelData.vertices[4].position = Manipulate;
		vertexData[1].position = Manipulate;
		vertexData[4].position = Manipulate;
	}
	ImGui::End();
#endif
}
