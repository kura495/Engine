#pragma once
class GamePlayState;
enum class GamePlayState {
	kTitle,
	kPlay,
	kClear,
	kOver,
};

class GamePlayStateState {
public:
	void Init(GamePlayState* PlayState);
	void Update(GamePlayState* PlayState);
	void Draw(GamePlayState* PlayState);
private:
};