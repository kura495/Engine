#pragma once
class GamePlayState;
class GamePlayStatePhase {
public:
	GamePlayStatePhase() {};
	virtual ~GamePlayStatePhase() {};
	virtual void Init(GamePlayState* PlayState) = 0;
	virtual void Update(GamePlayState* PlayState) = 0;
	virtual void Draw(GamePlayState* PlayState) = 0;
private:
};