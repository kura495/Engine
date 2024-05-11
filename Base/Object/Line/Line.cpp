#include "Line.h"


void Line::Init(){
	directX_ = DirectXCommon::GetInstance();
	srvManager_ = SRVManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * 2);
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 2;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position.x = 10.0f;
	vertexData[0].position.y = 10.0f;
	vertexData[0].position.z = 10.0f;
	vertexData[0].position.w = 1.0f;

	vertexData[1].position.x = 100.0f;
	vertexData[1].position.y = 100.0f;
	vertexData[1].position.z = 100.0f;
	vertexData[1].position.w = 1.0f;

	materialResource = directX_->CreateBufferResource(sizeof(Material));
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color.x = 1.0f;
	materialData->color.y = 1.0f;
	materialData->color.z = 1.0f;
	materialData->color.w = 1.0f;
}

void Line::Update(Vector4 stert,Vector4 end){
	vertexData[0].position = stert;
	vertexData[1].position = end;

}

void Line::Draw(const WorldTransform& transform, const ViewProjection& viewProjection){
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//matWorld
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff_->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_VS->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(5, viewProjection.constBuff_PS->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(0));

	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(2, 1, 0, 0);
}
