#pragma once
class Player;
class IPlayerState
{
public:
	IPlayerState();
	~IPlayerState();

	virtual void Init(Player* p);
	virtual void Update(Player* p);
private:

};

IPlayerState::IPlayerState()
{
}

IPlayerState::~IPlayerState()
{
}