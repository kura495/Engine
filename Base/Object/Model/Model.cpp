#include "Model.h"

void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
	directX_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();
	materialResource = directX_->CreateBufferResource(sizeof(Material));

	modelData_ = LoadModelFile(directoryPath,filename);

	//バッファリソースはLoadModelFileの中で作ってるよ
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	
	materialData->uvTransform = CreateIdentity4x4();

	materialData->enableLighting = lightFlag;
	materialData->color = color_;

}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection)
{
	//transform.constMap->matWorld = Multiply(transform.constMap->matWorld, modelData_.rootNode.localMatrix);

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData_.TextureIndex));
	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection, uint32_t TextureHundle)
{

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(TextureHundle));
	//Light
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
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
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };
				//aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
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
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();
	for (int Xit = 0; Xit < 4; Xit++) {
		for (int Yit = 0; Yit < 4; Yit++) {
			result.localMatrix.m[Xit][Yit] = aiLocalMatrix[Xit][Yit];
		}
	}
	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}
