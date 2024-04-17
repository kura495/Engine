#include "Animation.h"

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
