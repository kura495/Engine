#include "Filter.h"



void PPFilter::Update()
{
	if (Player::PushOptionButtern) {
		materialData->x = 1.0f;
	}
	else {
		materialData->x = 0.0f;	
	}
}
