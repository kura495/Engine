#include "Line.h"


void Line::Init(){
	directX_ = DirectXCommon::GetInstance();
	srvManager_ = SRVManager::GetInstance();
}

void Line::SetVertexData(std::vector<Vector4>& vertices){
	for (int it = 0; it < vertices.size();it++) {
		VertexData vertex;
		vertex.position = vertices[it];
		vertex.normal = { 0.0f,0.0f };
		vertex.texcoord = { 0.0f,0.0f };
		debugVertices_.push_back(vertex);
	}
}

void Line::Draw(const WorldTransform& transform, const ViewProjection& viewProjection){
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//matWorld
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff_->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(2, viewProjection.constBuff_VS->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, viewProjection.constBuff_PS->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(UINT(debugVertices_.size()), 1,0,0);
}

void Line::CreateBuffer()
{
	vertexResource = directX_->CreateBufferResource(sizeof(Vector4) * debugVertices_.size());
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * (uint32_t)debugVertices_.size();
	vertexBufferView.StrideInBytes = sizeof(Vector4);
	Vector4* vertexData{};
	vertexResource->Map(0 , nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, debugVertices_.data(), sizeof(Vector4) * debugVertices_.size());

	materialResource = directX_->CreateBufferResource(sizeof(Material));
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color.x = 0.0f;
	materialData->color.y = 0.0f;
	materialData->color.z = 0.0f;
	materialData->color.w = 1.0f;
}

void Line::UpdateVertexData(std::vector<Vector4>& vertices)
{
	Vector4* vertexData{};
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, vertices.data(), sizeof(Vector4) * vertices.size());
}