#include "Model.h"

void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
	directX_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();
	srvManager_ = SRVManager::GetInstance();

	modelData_ = LoadModelFile(directoryPath,filename);

	//バッファリソースはLoadModelFileの中で作ってるよ
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	indexResource = directX_->CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * (uint32_t)modelData_.indices.size();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex,modelData_.indices.data(),sizeof(uint32_t) * modelData_.indices.size());

	materialResource = directX_->CreateBufferResource(sizeof(Material));
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	
	materialData->uvTransform = CreateIdentity4x4();

	materialData->enableLighting = lightFlag;
	materialData->color = color_;

}

void Model::SkeletonUpdate(Skeleton& skeleton)
{
	// すべてのjointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.quaternion, joint.transform.translate);
		// 親がいれば親の行列を掛ける
		if (joint.parent) {
			joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Model::SkinClusterUpdate(SkinCluster& skinCluster,Skeleton& skeleton)
{
	// SkinClusterの更新

	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPaette[jointIndex].skeletonSpaceMatrix = Multiply(skinCluster.inverseBindPoseMatrices[jointIndex] , skeleton.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster.mappedPaette[jointIndex].skeletonSpaceInverseTransposeMatrix = Transpose((Inverse(skinCluster.mappedPaette[jointIndex].skeletonSpaceMatrix)));
	}

}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection)
{

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//IndexBuffer
	directX_->GetcommandList()->IASetIndexBuffer(&indexBufferView);
	//matWorld
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff_->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_VS->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(5, viewProjection.constBuff_PS->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData_.TextureIndex));

	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}
void Model::SkinDraw(const WorldTransform& transform, const ViewProjection& viewProjection, const SkinCluster& skinCluster)
{
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		vertexBufferView,
		skinCluster.influenceBufferView
	};

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 2, vbvs);
	//IndexBuffer
	directX_->GetcommandList()->IASetIndexBuffer(&indexBufferView);
	//matWorld
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform.constBuff_->GetGPUVirtualAddress());
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_VS->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(5, viewProjection.constBuff_PS->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData_.TextureIndex));
	//StructuredBuffer
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(6, skinCluster.paletteSRVHandle.GPU);
	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}

Model* Model::CreateModelFromObj(const std::string& directoryPath, const std::string& filename)
{
	Model* model=new Model();
	model->Initialize(directoryPath,filename);
	return model;
}

ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	// 名前が間違っているかも
	assert(scene->HasMeshes());//メッシュがないものには対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());//法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));//TexcoordがないMeshは今回は非対応
		//Index描画をするためにサイズを確保
		modelData.vertices.resize(mesh->mNumVertices);
		//ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);//三角形のみサポート
			//ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				//右手系->左手系への変換を忘れずに
				modelData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
				modelData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
				modelData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
			}
			//Indexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);
			}
		}
		// ここからBoneのデータを取得
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatirxAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatirxAssimp.Decompose(scale,rotate,translate);
			//1 左手系のBindPoseMatrixを作る
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix({ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			//InverseBindPoseMatrixを作る
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);
			// Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId});
			}
		}
	}
	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			modelData.TextureIndex = textureManager_->LoadTexture(modelData.material.textureFilePath);
		}
	}
	modelData.rootNode = ReadNode(scene->mRootNode);

	return modelData;
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData material;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + '/' + filename);//ファイルを開く
	assert(file.is_open());//開けなかったら止める
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			material.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return material;
}

Node Model::ReadNode(aiNode* node)
{
	Node result;

	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale,rotate,translate);

	result.transform.scale = { scale.x,scale.y,scale.z };
	result.transform.quaternion = { rotate.x,-rotate.y,-rotate.z,rotate.w };
	result.transform.translate = { -translate.x,translate.y,translate.z };

	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.quaternion, result.transform.translate);

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

Skeleton Model::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton;

	skeleton.root = CreateJoint(rootNode,{},skeleton.joints);

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name,joint.index);
	}

	return skeleton;
}

int32_t Model::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = CreateIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		//子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child,joint.index,joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
}

void Model::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	for (Joint& joint : skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames,animationTime);
			joint.transform.quaternion = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames,animationTime);
			joint.transform.translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames,animationTime);
		}
	}
}

SkinCluster Model::CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData)
{
	SkinCluster skinCluster;
	// palette用Resource
	skinCluster.paletteResource = directX_->CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource.Get()->Map(0,nullptr,reinterpret_cast<void**>(&mappedPalette));
	// spanを使ってアクセス
	skinCluster.mappedPaette = { mappedPalette, skeleton.joints.size() };
	skinCluster.paletteSRVHandle = srvManager_->GetDescriptorHandle();
	skinCluster.paletteSRVHandle.CPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	skinCluster.paletteSRVHandle.GPU.ptr += directX_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
#pragma region palette用のSRV
	// palette用のSRVを作成。StructeredBufferでアクセスできるようにする
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	directX_->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSRVHandle.CPU);
#pragma endregion

#pragma region Influence用のVBV
	// Influence用Resource
	skinCluster.influenceResource = directX_->CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
	VertexInfluence* mappedInflence = nullptr;
	skinCluster.influenceResource->Map(0,nullptr,reinterpret_cast<void**>(&mappedInflence));
	std::memset(mappedInflence,0,sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.mappedInfluence = { mappedInflence,modelData.vertices.size() };
	// Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);
#pragma endregion

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(),skinCluster.inverseBindPoseMatrices.end(),CreateIdentity4x4);

	// modelDataを解析してInfluenceを埋める
	// Modelの中のSkinClusterの情報を解析
	for (const auto& jointWeight : modelData.skinClusterData) {
		// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか調べる
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) {
			//同じ名前のjointがなかったので次へ
			continue;
		}
		// (*it).secondにはjointのindexが入っているので、該当のIndexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			// 該当のVertexIndexのinfluenceのinfluence情報を参照しておく
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			// 空いているところに入れる
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				// weight == 0 が空いている状態 その場所に代入
				if (currentInfluence.weights[index] == 0.0f) {
					// Weightを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					// jointのIndexを代入
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
}

std::vector<Model*> Model::SkeletonDebugInit(Skeleton& skeleton, std::vector<WorldTransform>& worlds)
{
	worlds.resize(skeleton.joints.size());
	for (int it = 0; it < skeleton.joints.size(); it++) {

	}
}

void Model::SkeletonDebugUpdate(Skeleton& skeleton, const Animation& animation, float animationTime, std::vector<WorldTransform>& worlds)
{
	for (Joint& joint : skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			worlds[joint.index].transform_.scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime);
			worlds[joint.index].transform_.quaternion = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime);
			worlds[joint.index].transform_.translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime);
		}
	}
}
