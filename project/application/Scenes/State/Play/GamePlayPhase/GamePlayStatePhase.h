#pragma once
class GamePlayState;
enum class GamePlayPhase {
	kTitle,
	kPlay,
	kClear,
	kOver,
};

class GamePlayStatePhase {
public:
	virtual void Init(GamePlayState* PlayState) = 0;
	virtual void Update(GamePlayState* PlayState) = 0;
	virtual void Draw(GamePlayState* PlayState) = 0;
private:
};