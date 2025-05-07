#include "StartAnime.h"
#include "UFO/UFO.h"
void StartAnime::Init(UFO* ufo)
{
	ufo;
}
void StartAnime::Update(UFO* ufo)
{
	ufo->GetWorld().transform.translate.z += 0.5f;
	ufo->particle_->CustumSpawnParticle(ufo->particleEmitter);
	ufo->particle_->Update();
}
void StartAnime::Draw(UFO* ufo)
{
	ufo->particle_->RendererDraw();
}
