#pragma once
#include "../IPlayerState.h"
class PDead : public IPlayerState
{
public:
	PDead();
	~PDead() { Input::VibrateController(0, 0, 0); };

	void Init(Player* player)override;
	void Update(Player* player)override;
	void Draw(Player* player)override;
	std::string ShowState()override;
private:
	void NormalDeadUpdate(Player* player);
	void SlamDeadUpdate(Player* player);
	//モデルを描画するか
	bool isModelDraw = true;
	//パーティクル関連
	std::unique_ptr<ParticleSystem> deadParticle_;
	Particle CustomParticle();
	void UpdatedeadParticle(Particle& particle);
	Emitter deadParticleEmitter;
	float easeT = 0;
	//アニメーション
	std::unique_ptr<Animation> deadAnimation;
};

