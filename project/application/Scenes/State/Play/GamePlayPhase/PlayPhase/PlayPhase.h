#pragma once
#include "State/Play/GamePlayPhase/GamePlayStatePhase.h"

class PlayPhase : public GamePlayStatePhase {
public:
	~PlayPhase() { Input::VibrateController(0, 0, 0); }
	
	void Init(GamePlayState* playState)override;
	void Update(GamePlayState* playState)override;
	void Draw(GamePlayState* playState)override;

	static void HitStop(float second);
	static float HitStopCount;
	static float TargetTime;
private:
	Vector2 moterValue;
	bool HitStopUpdate();

};