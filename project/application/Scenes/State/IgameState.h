#pragma once
//StatePatternの基底クラス(インターフェイス型)
enum GameStateNo {
	TITLE,
	PLAY,
	CLEAR,
	GAMEOVER,
	GameStateMax
};
class IGameState
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	int GetSceneNum() { return StateNo; }

	virtual  ~IGameState() {};
protected:
	static int StateNo;
};
