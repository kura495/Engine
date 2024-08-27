#include "LockOn.h"

void LockOn::Init()
{
	input = Input::GetInstance();
}

void LockOn::Update(const std::list<Enemy*>& enemies)
{
	input->GetJoystickState(joyState);

	if (target_) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (!(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				target_ = nullptr;
			}
		}
		else if (ChackOnLockOnRenge(Renderer::viewProjection)) {
			target_ = nullptr;
		}
		//Yを押したらターゲット変更
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			if (!(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
				if (iteratornum > 0) {
					iteratornum--;
					Search(enemies, Renderer::viewProjection);
					ChangeTarget();
				}
				else {
					iteratornum = max;
					Search(enemies, Renderer::viewProjection);
					ChangeTarget();
				}
			}
		}
	}
	else {
		//入力があったらターゲットを検索
		//TODO:今は無効化
		/*if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (!(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				Search(enemies, Renderer::viewProjection);
			}
		}*/
	}

	if (target_) {
		//敵のロックオン座標取得
		//ずれているので調整
		Vector3 positionWorld = {target_->GetPos()};
	}
	joyStatePre = joyState;
}

Vector3 LockOn::GetTargetPosition() const
{
	if (target_) {
		return target_->GetPos();
	}
	return Vector3();
}

void LockOn::Search(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection)
{
	targets.clear();
	//全ての敵に対して順にロックオン判定
	for (Enemy* enemy_ : enemies) {
		if (!enemy_->GetIsAlive()) {
			continue;
		}
		//敵のロックオン座標取得
		Vector3 positionWorld = enemy_->GetPos();

		//ワールド→ビュー座標変換
		Vector3 positionView = VectorTransform(positionWorld, viewProjection.matView);

		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			//カメラ前方との角度を計算
			float norm = Vector3::Length(positionView);
			float angleCosine = std::acos(positionView.z / norm);

			//角度条件チェック(コーンにおさまっているか)
			if (std::abs(angleCosine) <= angleRange_) {
				targets.emplace_back(std::make_pair(positionView.z, enemy_));
			}
		}
	}
	Target();

}

void LockOn::Target() {
	target_ = nullptr;

	if (!targets.empty()) {
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first > pair2.first; });
		max = (int)targets.size();
		target_ = targets.front().second;
	}
}

void LockOn::ChangeTarget()
{

	if (!targets.empty()) {
		targets.sort([](auto& pair1, auto& pair2) { return pair1.first > pair2.first; });
		max = (int)targets.size();
		target_ = targets.front().second;

		auto it = targets.begin(); // イテレータをリストの先頭に設定する
		if (iteratornum >= targets.size()) {
			iteratornum = (int)targets.size();
		}
		//iteratorを回数分進める
		std::advance(it, iteratornum);
		if (it != targets.end()) {
			std::pair<float,Enemy*>element = *it;
			target_ = element.second;
		}
	}
}


Vector3 LockOn::WorldToScreen(Vector3& position, const ViewProjection& viewProjection)
{
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjctionViewport = Matrix4x4::Multiply(viewProjection.matView, Matrix4x4::Multiply(viewProjection.matProjection, matViewport));
	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 position_ = VectorTransform(position, matViewProjctionViewport);

	return position_;
}

bool LockOn::ChackOnLockOnRenge(const ViewProjection& viewProjection)
{
	//敵のロックオン座標取得
	Vector3 positionWorld = target_->GetPos();

	//ワールド→ビュー座標変換
	Vector3 positionView = VectorTransform(positionWorld, viewProjection.matView);

	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		//カメラ前方との角度を計算
		float norm = Vector3::Length(positionView);
		float angleCosine = std::acos(positionView.z / norm);

		//角度条件チェック(コーンにおさまっているか)
		if (std::abs(angleCosine) <= angleRange_) {
			return false;
		}
	}
	return true;
}