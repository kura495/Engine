#pragma once
#include "Model/Model.h"
#include "Input/Input.h"

#include "UFO/state/Idle/Idle.h"
#include "UFO/state/Normal/Normal.h"
#include "UFO/state/Boost/Boost.h"
#include "UFO/state/StartAnime/StartAnime.h"
class UFO
{
public:
	UFO() {};
	~UFO() {};

	void Init();
	void Update();
	void Draw();

	void ImGui();

	WorldTransform& GetWorld() { return world_; };
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

	std::unique_ptr<ParticleSystem> particle_;
	Emitter particleEmitter;
private:
	//UFOのモデル
	std::unique_ptr<Model> model_;
	WorldTransform world_;
	//ステート
	std::unique_ptr<IUFOState> state_;
	//SRVが解放できないため
	//パーティクル関連

	Particle CustomParticle();
	void UpdateParticle(Particle& particle);
	void UpdateParticle2(Particle& particle);

};
