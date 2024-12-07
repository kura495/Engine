#include "ParticleSystem.h"

void ParticleSystem::Initalize(const std::string filePath)
{
	textureManager_ = TextureManager::GetInstance();
	directX_ = DirectXCommon::GetInstance();
	sRVManager_ = SRVManager::GetInstance();

	modelData.vertices.push_back({ .position = { -1.0f,1.0f,0.0f,1.0f },.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},   .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},	.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},	.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = filePath;
	int Texture = textureManager_->LoadTexture(modelData.material.textureFilePath);
	modelData.TextureIndex = Texture;

	CreateResources();
	CreateSRV();	

	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	materialData->enableLighting = false;
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->uvTransform = Matrix4x4::CreateIdentity();

	Pipeline_ = std::make_unique<ParticlePipeLine>();
	Pipeline_->Initalize();

	TestField.acceleration = {0.0f,0.0f,0.0f};
	TestField.area.min = {-1.0f,-1.0f,-1.0f};
	TestField.area.max = { 1.0f,1.0f,1.0f };
	
}

void ParticleSystem::Update()
{
	numInstance = 0;

	Matrix4x4 billboardMatrix = Renderer::viewProjection.CameraMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (std::list<Particle>::iterator particleIt = particles.begin(); particleIt != particles.end();) {

		if ((*particleIt).lifeTime <= (*particleIt).currentTime) {
			particleIt = particles.erase(particleIt);
			continue;
		}

		UpdateParticle(*particleIt);

		if (numInstance < kNumMaxInstance) {
			instancinsData[numInstance].matWorld = Matrix4x4::Multiply((*particleIt).matWorld, billboardMatrix);
			instancinsData[numInstance].matWorld.m[3][0] = (*particleIt).transform.translate.x;
			instancinsData[numInstance].matWorld.m[3][1] = (*particleIt).transform.translate.y;
			instancinsData[numInstance].matWorld.m[3][2] = (*particleIt).transform.translate.z;
			instancinsData[numInstance].color = (*particleIt).color;
			++numInstance;
		}
		++particleIt;
	}

#ifdef _DEBUG
	ImGui();
#endif

}

void ParticleSystem::RendererDraw()
{
	Renderer::AddParticleData(*this);
}

void ParticleSystem::Draw(const ViewProjection& viewProjection)
{
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(2, viewProjection.constBuff_VS->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(3, textureManager_->GetGPUHandle(modelData.TextureIndex));
	
	//インスタンシング用WVP
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(1, textureSrvHandle.GPU);

	directX_->GetcommandList()->DrawInstanced(6, numInstance, 0, 0);
}

void ParticleSystem::SpawnParticle(Emitter& emitter, std::mt19937& randomEngine)
{
	particles.splice(particles.end(), Emit(emitter, randomEngine));
}

void ParticleSystem::CustumSpawnParticle(Emitter& emitter)
{
	particles.splice(particles.end(), CustumEmit(emitter));
}

std::list<Particle> ParticleSystem::CustumEmit(Emitter& emitter)
{
	std::list<Particle> Emitparticles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		Emitparticles.push_back(CustumSpawn());
	}
	return Emitparticles;
}

void ParticleSystem::PreDraw()
{
	directX_->GetcommandList()->SetGraphicsRootSignature(Pipeline_->GetPSO().rootSignature.Get());
	directX_->GetcommandList()->SetPipelineState(Pipeline_->GetPSO().graphicsPipelineState.Get());
}

std::list<Particle> ParticleSystem::Emit(Emitter& emitter, std::mt19937& randomEngine)
{
	std::list<Particle> Emitparticles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		Emitparticles.push_back(MakeNewParticle(emitter,randomEngine));
	}
	return Emitparticles;
}

bool ParticleSystem::IsCollision(const AABBData& aabb, const Vector3& point)
{
	if (aabb.min.x < point.x && point.x < aabb.max.x &&
		aabb.min.y < point.y && point.y < aabb.max.y &&
		aabb.min.z < point.z && point.z < aabb.max.z) {

		return true;
	}
	return false;
}

void ParticleSystem::ImGui()
{
	ImGui::Begin("Emitter");
	ImGui::InputFloat3("translate",&Testemitter.world_.transform.translate.x);
	ImGui::End();
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
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	textureSrvHandle = sRVManager_->GetDescriptorHandle();

	//ImGui分
	textureSrvHandle.CPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandle.GPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	directX_->GetDevice()->CreateShaderResourceView(InstancingResource.Get(),&instancingSrvDesc, textureSrvHandle.CPU);
}

Particle ParticleSystem::MakeNewParticle(Emitter& emitter,std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	Particle particle;
	Vector3 ramdomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
	particle.transform.translate = ramdomTranslate + emitter.world_.transform.translate;
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
