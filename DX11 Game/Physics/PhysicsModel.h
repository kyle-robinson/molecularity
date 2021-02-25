#pragma once
#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H

#include "RenderableGameObject.h"

/// <summary>
/// Managers the generic physics calculations for a given object.
/// </summary>
class PhysicsModel
{
public:
	PhysicsModel( RenderableGameObject* transform );
	virtual ~PhysicsModel() = default;

	//get/set forces
	XMFLOAT3 GetVelocity() const noexcept { return mVelocity; }
	void SetVelocity( XMFLOAT3 velocity ) noexcept { mVelocity = velocity; }

	XMFLOAT3 GetAcceleration() const noexcept { return mAcceleration; }
	void SetAcceleration( XMFLOAT3 acceleration ) noexcept { mAcceleration = acceleration; }

	//functions for updating forces and object position
	virtual void Update( const float dt );
	void AddWeight();
	void CalculateAcceleration();
	void CalculateVelocity( const float dt );
	void AddFriction( const float dt );
	void AddDrag();
	void LaminarDrag();
	void TurbulentDrag();
	void CalculatePosition( const float dt );
	void AddGravity();

	float Magnitude( XMFLOAT3 vec ) const noexcept
	{
		return ( sqrtf(
			powf( vec.x, 2 ) +
			powf( vec.y, 2 ) +
			powf( vec.z, 2 ) )
		);
	}
	XMFLOAT3 Normalization( XMFLOAT3 vec ) const noexcept;
private:
	// Constants
	static constexpr float mGravity = 9.81f;
	static constexpr float mDragFactor = 0.75f;
	static constexpr float mFrictionFactor = 0.0002f;
	// Local Variables
	float mMass;
	float mWeight;
	bool mUseLaminar;
	XMFLOAT3 mDrag;
	XMFLOAT3 mFriction;
	XMFLOAT3 mPosition;
	XMFLOAT3 mNetForce;
	XMFLOAT3 mVelocity;
	XMFLOAT3 mAcceleration;
	RenderableGameObject* mTransform;
};

#endif