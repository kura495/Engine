#pragma once
#include "../IPlayerState.h"
class PDead : public IPlayerState
{
public:
	PDead();
	~PDead() { Input::VibrateController(0, 0, 0); };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
private:
	void NormalDeadUpdate(Player* p);
	void SlamDeadUpdate(Player* p);
	//モデルを描画するか
	bool isModelDraw = true;
	//パーティクル関連
	std::unique_ptr<ParticleSystem> deadParticle_;
	void UpdatedeadParticle(Particle& particle);
	Emitter deadParticleEmitter;
	//アニメーション
	std::unique_ptr<Animation> deadAnimation;
};

