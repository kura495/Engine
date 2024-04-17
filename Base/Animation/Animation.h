#pragma once
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "KeyFrame/Struct/KeyFrame_Struct.h"
#include "Base/Math/Vector/VectorCalc.h"

// アニメーション構造体
class Animation {
public:

	static Animation LoadAnimationFile(const std::string& directrypath,const std::string& filename);
	static Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes,float time);
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes,float time);

	float duration; // アニメーション全体の尺(単位は秒)
	// NodeAnimationの集合　Node名で検索できるようにする
	std::map<std::string, NodeAnimation> nodeAnimations;
private:

};