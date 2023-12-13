#include "ParticleSystem.h"

void ParticleSystem::Initalize(int particleVolume,const std::string filePath)
{
	textureManager_ = TextureManager::GetInstance();
	directX_ = DirectXCommon::GetInstance();

	modelData.vertices.push_back({ .position = { -1.0f,1.0f,0.0f,1.0f },.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},   .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},	 .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},	 .texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = filePath;
	int Texture = textureManager_->LoadTexture(modelData.material.textureFilePath);
	modelData.TextureIndex = Texture;
	particleVolume_ = particleVolume;

	CreateResources();
	CreateSRV();	

	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	for (uint32_t Volume_i = 0; Volume_i < kNumMaxInstance; Volume_i++) {
		particles[Volume_i] = MakeNewParticle(randomEngine);
	}

	materialData->enableLighting = false;
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->uvTransform = CreateIdentity4x4();

	Pipeline_ = std::make_unique<ParticlePipeLine>();
	Pipeline_->Initalize();

}
void ParticleSystem::Initalize(int particleVolume,const std::string filePath, Vector3 Pos)
{
	textureManager_ = TextureManager::GetInstance();
	directX_ = DirectXCommon::GetInstance();

	modelData.vertices.push_back({ .position = { -1.0f,1.0f,0.0f,1.0f },.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},  .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = filePath;
	int Texture = textureManager_->LoadTexture(modelData.material.textureFilePath);
	modelData.TextureIndex = Texture;
	particleVolume_ = particleVolume;

	CreateResources();
	CreateSRV();	

	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	for (uint32_t Volume_i = 0; Volume_i < kNumMaxInstance; Volume_i++) {
		particles[Volume_i] = MakeNewParticle(randomEngine);
	}

	materialData->enableLighting = false;
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->uvTransform = CreateIdentity4x4();

	Pipeline_ = std::make_unique<ParticlePipeLine>();
	Pipeline_->Initalize();

	SetPos(Pos);

}

void ParticleSystem::Update(const ViewProjection& viewProjection)
{
	numInstance = 0;
	Matrix4x4 billboardMatrix = viewProjection.CameraMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (uint32_t Volume_i = 0; Volume_i < kNumMaxInstance; Volume_i++) {
		if (particles[Volume_i].lifeTime <= particles[Volume_i].currentTime) {
			continue;
		}
		Vector3 velcity = particles[Volume_i].velocity * kDeltaTime;
		particles[Volume_i].translate += velcity;
		float alpha = 1.0f - (particles[Volume_i].currentTime / particles[Volume_i].lifeTime);
		particles[Volume_i].color.w = alpha;
		particles[Volume_i].currentTime += kDeltaTime;
		particles[Volume_i].matWorld = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, particles[Volume_i].translate);

		instancinsData[Volume_i].matWorld = Multiply(particles[Volume_i].matWorld, billboardMatrix);
		instancinsData[Volume_i].color = particles[Volume_i].color;
		++numInstance;
	}


}

void ParticleSystem::Draw(const ViewProjection& viewProjection)
{
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(2, viewProjection.constBuff_->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(3, textureManager_->GetGPUHandle(modelData.TextureIndex));
	
	//インスタンシング用WVP
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(1, instancingSRVHandleGPU);

	directX_->GetcommandList()->DrawInstanced(6, numInstance, 0, 0);
}

void ParticleSystem::PreDraw()
{
	directX_->GetcommandList()->SetGraphicsRootSignature(Pipeline_->GetPSO().rootSignature.Get());
	directX_->GetcommandList()->SetPipelineState(Pipeline_->GetPSO().graphicsPipelineState.Get());
}

void ParticleSystem::SetPos(Vector3 Pos)
{
	//for (uint32_t Volume_i = 0; Volume_i < kNumMaxInstance; Volume_i++) {
	//	//particles[Volume_i].translate = Pos;
	//	
	//} 
	Pos;
}

void ParticleSystem::CreateResources()
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
	//Create InstancingResources
	InstancingResource = directX_->CreateBufferResource(sizeof(ParticleForGPU)* kNumMaxInstance);
	//maping InstancingResources
	InstancingResource->Map(0,nullptr,reinterpret_cast<void**>(&instancinsData));
	
}

void ParticleSystem::CreateSRV()
{
	uint32_t descriptorSizeSRV = directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	instancingSRVHandleCPU = GetCPUDescriptorHandle(directX_->GetsrvDescriptorHeap(), descriptorSizeSRV, 100);
	instancingSRVHandleGPU = GetGPUDescriptorHandle(directX_->GetsrvDescriptorHeap(), descriptorSizeSRV, 100);
	directX_->GetDevice()->CreateShaderResourceView(InstancingResource.Get(),&instancingSrvDesc, instancingSRVHandleCPU);
}

D3D12_CPU_DESCRIPTOR_HANDLE ParticleSystem::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE ParticleSystem::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

Particle ParticleSystem::MakeNewParticle(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	Particle particle;
	particle.translate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particle.color = MakeParticleColor(randomEngine);
	particle.lifeTime = MakeParticleLifeTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}
Vector4 ParticleSystem::MakeParticleColor(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) 
	,distribution(randomEngine) };
}
float ParticleSystem::MakeParticleLifeTime(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(1.0f, 3.0f);
	return distribution(randomEngine);
}
