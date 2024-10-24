#pragma once

#include "Renderer/Renderer.h"
#include "Game/Object/Enemy/Enemy.h"
#include "Input/Input.h"

class LockOn
{
public:
	void Init();
	void Update(const std::list<Enemy*>& enemies);

	Vector3 GetTargetPosition()const;
	bool ExistTarget()const { return target_ ? true : false; }

private:
	void Search(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection);
	Vector3 WorldToScreen(Vector3& position, const ViewProjection& viewProjection);
	bool ChackOnLockOnRenge(const ViewProjection& viewProjection);
	void Target();
	void ChangeTarget();

	//目標
	std::list<std::pair<float,Enemy*>> targets;

	Input* input = nullptr;
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	int iteratornum;
	int max = 0;

	Enemy* target_ = nullptr;

	float kDegreeToRadian = (float)std::numbers::pi / 180;
	float minDistance_ = 10.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 20.0f * kDegreeToRadian;
};