#include "stdafx.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem( std::shared_ptr<RenderableGameObject> parent, int maxParticles, float particleSize )
	: mParent( parent )
{
	for ( unsigned int i = 0; i < maxParticles; i++ )
		mParticles.push_back( std::make_shared<Particle>( this ) );
}

ParticleSystem::ParticleSystem( XMMATRIX position, int maxParticles, float particleSize )
	: mPosition( position )
{
	mParent = nullptr;

	for ( unsigned int i = 0; i < maxParticles; i++ )
		mParticles.push_back( std::make_shared<Particle>( this ) );
}

void ParticleSystem::Draw()
{
	for( const auto &particle : mParticles )
		particle->Draw();
}

void ParticleSystem::Update( const float dt )
{
	for ( const auto &particle : mParticles )
		particle->Update( dt );
}