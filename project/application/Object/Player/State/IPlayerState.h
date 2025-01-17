#pragma once
#include <string>
class Player;
class IPlayerState
{
public:
	IPlayerState() {};
	virtual ~IPlayerState() {};

	virtual void Init(Player* p) = 0;
	virtual void Update(Player* p) = 0;
	virtual std::string ShowState() = 0;
private:

};