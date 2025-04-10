#include "Normal.h"
#include "UFO/UFO.h"
void Normal::Init(UFO* ufo)
{
	ufo;

}
void Normal::Update(UFO* ufo)
{
	ufo->GetWorld().transform.translate = homePosition + random::Generate(-0.01f,0.01f);

	ufo->particle_->CustumSpawnParticle(ufo->particleEmitter);
	ufo->particle_->Update();

	if (Input::GetPadTrreger().x == 255 && Input::GetPadTrreger().y == 255) {
		ufo->ChangeState<Boost>();
	}
	else if (Input::GetPadTrreger().x == 0 && Input::GetPadTrreger().y == 0) {
		ufo->ChangeState<Idle>();
	}
}
void Normal::Draw(UFO* ufo)
{
	ufo->particle_->RendererDraw();
}

