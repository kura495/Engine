#include "SkyBox.h"
#include "Renderer/Renderer.h"
void SkyBox::Init()
{
	directX_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	//バーテックスデータ
	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * 24);
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	MakeVertexBufferView();
	//インデックスリソース
	indexResource = directX_->CreateBufferResource(sizeof(uint32_t) * 36);
	indexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	MakeIndexBufferView();
	//マテリアルリソース
	materialResource = directX_->CreateBufferResource(sizeof(Material));
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//マテリアルの設定
	materialData->uvTransform = Matrix4x4::CreateIdentity();
	materialData->enableLighting = false;
	materialData->color = {1.0f,1.0f,1.0f,1.0f};


	CreateResource();
}

void SkyBox::RendererDraw(const WorldTransform& transform)
{
	transform;
}

void SkyBox::Draw(const WorldTransform& transform)
{
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	directX_->GetcommandList()->IASetIndexBuffer(&indexBufferView);
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//WorldTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff_.Get()->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(2, Renderer::viewProjection.constBuff_VS->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(3, textureManager_->GetGPUHandle(TextureHandle));


	directX_->GetcommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0);
}


void SkyBox::CreateResource()
{
#pragma region
	vertexData[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[3].position = { 1.0f,-1.0f,-1.0f,1.0f };

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 2;
	indexData[4] = 1;
	indexData[5] = 3;

#pragma endregion 右面

#pragma region
	vertexData[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[7].position = { -1.0f,-1.0f,1.0f,1.0f };

	indexData[6] = 4;
	indexData[7] = 5;
	indexData[8] = 6;
	indexData[9] = 6;
	indexData[10] = 5;
	indexData[11] = 7;

#pragma endregion 左面

#pragma region
	vertexData[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[11].position = { 1.0f,-1.0f,1.0f,1.0f };

	indexData[12] = 8;
	indexData[13] = 9;
	indexData[14] = 10;
	indexData[15] = 10;
	indexData[16] = 9;
	indexData[17] = 11;

#pragma endregion 後ろ面

#pragma region
	vertexData[12].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[13].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[14].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[15].position = { 1.0f,-1.0f,-1.0f,1.0f };

	indexData[18] = 13;
	indexData[19] = 12;
	indexData[20] = 14;
	indexData[21] = 13;
	indexData[22] = 14;
	indexData[23] = 15;

#pragma endregion 前面

#pragma region
	vertexData[16].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[17].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[18].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[19].position = { 1.0f,1.0f,-1.0f,1.0f };

	indexData[24] = 16;
	indexData[25] = 17;
	indexData[26] = 18;
	indexData[27] = 18;
	indexData[28] = 17;
	indexData[29] = 19;

#pragma endregion 上面

#pragma region
	vertexData[20].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[21].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[22].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[23].position = { 1.0f,-1.0f,-1.0f,1.0f };

	indexData[30] = 20;
	indexData[31] = 22;
	indexData[32] = 21;
	indexData[33] = 22;
	indexData[34] = 23;
	indexData[35] = 21;

#pragma endregion 下面
}

void SkyBox::MakeVertexBufferView()
{
	//リソースの先頭のアドレス
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	//使用する頂点サイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) *24;
	//1頂点あたりのアドレス
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}
void SkyBox::MakeIndexBufferView()
{
	//リソース先頭アドレス
	indexBufferView.BufferLocation = indexResource.Get()->GetGPUVirtualAddress();
	//使用するインデックスサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 36;
	//インデックスはuint32_t
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}