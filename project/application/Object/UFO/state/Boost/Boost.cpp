#include "Boost.h"
#include "UFO/UFO.h"
void Boost::Init(UFO* ufo)
{
	ufo;
}
void Boost::Update(UFO* ufo)
{
	ufo->GetWorld().transform.translate = homePosition + random::Generate(-0.2f, 0.2f);

	ufo->particle_->CustumSpawnParticle(ufo->particleEmitter);
	ufo->particle_->Update();

	if (Input::GetPadTrreger().x <=254 && Input::GetPadTrreger().y <= 254) {
		ufo->GetWorld().transform.translate = homePosition;
		ufo->ChangeState<Normal>();
	}
}
void Boost::Draw(UFO* ufo)
{
	ufo->particle_->RendererDraw();
}
