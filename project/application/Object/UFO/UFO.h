#pragma once
#include "Model/Model.h"
#include "Input/Input.h"

#include "UFO/state/Idle/Idle.h"
#include "UFO/state/Normal/Normal.h"
#include "UFO/state/Boost/Boost.h"
class UFO
{
public:
	UFO() {};
	~UFO() {};

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	template <typename T>
	void ChangeState() {
		state_ = std::make_unique<T>();
		//ステートのすべての処理が終わる前にステートを切り替えてしまうと、
		//ステートが変わって後の処理が出来なくなってエラーが出る
		state_->Init(this);
	}
	UFOState GetState() { return state_->GetStateType(); }

private:
	//UFOのモデル
	std::unique_ptr<Model> UFOmodel_;
	WorldTransform UFOWorld_;
	//ステート
	std::unique_ptr<IUFOState> state_;
};
