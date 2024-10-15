#pragma once
#include "Scenes/State/IgameState.h"
#include "Common/Engine/MyEngine.h"
#include "ModelData.h"
#include "Audio.h"
#include "Input.h"
#include "Object/Mesh/Mesh.h"
#include "Sprite/Sprite.h"
#include "Light/Light.h"
#include "Object/Sphere/Sphere.h"
#include "Texture/TextureManager.h"
#include "Object/Model/Model.h"
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Camera/DebugCamera/DebugCamera.h"
#include "WorldTransform/WorldTransform.h"
#include "ViewProj/ViewProjection.h"
#include "Math_Structs.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Collider/Manager/CollisionManager.h"
#include "Editer/Editer.h"
#include "Object/Manager/ObjectManager.h"
#include "Animation/Animation.h"
#include "Renderer/Renderer.h"
#include "Object/Line/Line.h"

#include "Game/Object/Camera/FollowCamera.h"

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void BehaviorUpdate();
	void Draw();

private:

};