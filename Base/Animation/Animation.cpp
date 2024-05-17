#include "Animation.h"

void Animation::Init(){
	directX_ = DirectXCommon::GetInstance();
	srvManager_ = SRVManager::GetInstance();
}

Animation Animation::LoadAnimationFile(const std::string& directrypath, const std::string& filename)
{
	Animation animation;// 今回作るアニメーション
	Assimp::Importer importer;
	std::string filePath = directrypath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(),0);
	assert(scene->mNumAnimations != 0);// アニメーションがない
	aiAnimation* animationAssimp = scene->mAnimations[0];//この状態だと最初のアニメーションだけ読み込む
	// mTicksPerSecond = 周波数
	// mDuration = 周波数における長さ
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);// 時間を秒に変換　
	// NodeAnimationを解析
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		// translate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyFrameVector3 keyFrame;
			keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここでも秒に変換
			keyFrame.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };// 右手->左手
			nodeAnimation.translate.keyFrames.push_back(keyFrame);
		}
		// rotate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyFrameQuaternion keyFrame;
			keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここでも秒に変換
			keyFrame.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };// 右手->左手
			nodeAnimation.rotate.keyFrames.push_back(keyFrame);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyFrameVector3 keyFrame;
			keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここでも秒に変換
			keyFrame.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };// 右手->左手
			nodeAnimation.scale.keyFrames.push_back(keyFrame);
		}
	}
	return animation;
}

Vector3 Animation::CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size()== 1||time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return VectorLerp(keyframes[index].value,keyframes[nextIndex].value,t);
		}
	}

	return (*keyframes.rbegin()).value;
}

Quaternion Animation::CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

void Animation::SkeletonUpdate(Skeleton& skeleton)
{
	// すべてのjointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.quaternion, joint.transform.translate);
		// 親がいれば親の行列を掛ける
		if (joint.parent) {
			joint.skeletonSpaceMatrix = Matrix4x4::Multiply(joint.localMatrix, skeleton.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Animation::SkinClusterUpdate(SkinCluster& skinCluster, Skeleton& skeleton)
{
	// SkinClusterの更新

	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPaette[jointIndex].skeletonSpaceMatrix = Matrix4x4::Multiply(skinCluster.inverseBindPoseMatrices[jointIndex], skeleton.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster.mappedPaette[jointIndex].skeletonSpaceInverseTransposeMatrix = Matrix4x4::Transpose((Matrix4x4::Inverse(skinCluster.mappedPaette[jointIndex].skeletonSpaceMatrix)));
	}

}

Skeleton Animation::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton;

	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	//名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

int32_t Animation::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::CreateIdentity();
	joint.transform.scale = node.transform.scale;
	joint.transform.quaternion = node.transform.quaternion;
	joint.transform.translate = node.transform.translate;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		//子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
}

void Animation::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	for (Joint& joint : skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime);
			joint.transform.quaternion = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime);
			joint.transform.translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime);
		}
	}
}

SkinCluster Animation::CreateSkinCluster(const Skeleton& skeleton, const ModelData& modelData)
{
	SkinCluster skinCluster;
	// palette用Resource
	skinCluster.paletteResource = directX_->CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
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
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInflence));
	std::memset(mappedInflence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.mappedInfluence = { mappedInflence,modelData.vertices.size() };
	// Influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);
#pragma endregion

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), Matrix4x4::CreateIdentity);

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

void Animation::CreateBoneLineVertices(const Skeleton& skeleton, int32_t parentIndex, std::vector<Vector4>& vertices)
{
	const Joint& parentJoint = skeleton.joints[parentIndex];
	for (int32_t childIndex : parentJoint.children)
	{
		const Joint& childJoint = skeleton.joints[childIndex];
		vertices.push_back({ parentJoint.skeletonSpaceMatrix.m[3][0],parentJoint.skeletonSpaceMatrix.m[3][1],parentJoint.skeletonSpaceMatrix.m[3][2],1.0f });
		vertices.push_back({ childJoint.skeletonSpaceMatrix.m[3][0],childJoint.skeletonSpaceMatrix.m[3][1],childJoint.skeletonSpaceMatrix.m[3][2],1.0f });
		CreateBoneLineVertices(skeleton, childIndex, vertices);
	}
}