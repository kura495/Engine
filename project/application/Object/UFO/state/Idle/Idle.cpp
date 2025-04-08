#include "Idle.h"
#include "UFO/UFO.h"
void Idle::Init(UFO* ufo)
{
	ufo;
}
void Idle::Update(UFO* ufo)
{
	if (Input::GetPadTrreger().x >= 1 && Input::GetPadTrreger().y >= 1) {
		ufo->ChangeState<Normal>();
	}
}
void Idle::Draw(UFO* ufo)
{
	ufo;
}
