#include "Normal.h"

void PPNormal::Update()
{
	if (Player::PushOptionButtern) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;
	}
}