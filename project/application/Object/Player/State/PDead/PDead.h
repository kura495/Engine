#pragma once
#include "../IPlayerState.h"
class PDead : public IPlayerState
{
public:
	PDead();

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
private:
	//死んだときにモデルを描画するか
	bool isDeadModelDraw = true;
	//パーティクル関連
	std::unique_ptr<ParticleSystem> deadParticle_;
	void UpdatedeadParticle(Particle& particle);
	Emitter deadParticleEmitter;
	//アニメーション
	std::unique_ptr<Animation> deadAnimation;
};

