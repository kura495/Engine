#pragma once
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include <algorithm>

class Bomb : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;
	/// <summary>
	/// ���e�𓊂��鎞�ɌĂяo���֐�
	/// </summary>
	/// <param name="targetVector">�^�[�Q�b�g�̃��[���h���W</param>
	void ThrowBomb(Vector3 targetVector) {
		isThrowFlag = true;
		//���������߂�
		Vector3 playerToBomb = targetVector - world_.transform.translate;
		//�����̒萔���|����
		forTargetVector = playerToBomb.Normalize() * kSpeedValue;
	};
private:
	//�v���C���[�Ɍ������Ă��������̔{��
	float kSpeedValue = 0.5f;
	//�v���C���[�����֌������x�N�g��
	Vector3 forTargetVector;
	//���������ǂ����̃t���O
	bool isThrowFlag = false;
	//�����͈�
	Vector3 boxMax;
	Vector3 boxMin;
#pragma region
	//�{�X�̎�_�̓����蔻��
	void ColliderInit();
	void OnCollision(const ICollider* colliderA);
	OBBoxCollider collider;
#pragma endregion Collider

};
