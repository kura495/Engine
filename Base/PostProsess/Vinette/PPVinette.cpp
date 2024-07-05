#include "PPVinette.h"

void PPVinette::Update()
{


	if (Player::playerMoveValue) {
		if (Player::PushOptionButtern) {
			return;
		}
		materialData->x = (std::min)(materialData->x + 0.01f,0.8f);
	}
	else {
		materialData->x = (std::max)(materialData->x - 0.01f, 0.0f);
	}
}
