#include "stdafx.h"
#include "PhysicsModel.h"

PhysicsModel::PhysicsModel( RenderableGameObject* transform )
	: mTransform( transform )
{
	mMass = 100.0f;
	mUseLaminar = false;
	mPosition = mTransform->GetPositionFloat3();

	// object weight = mass * gravity
	mWeight = mMass * mGravity;

	mDrag = { 0.0f, 0.0f, 0.0f };
	mFriction = { 0.0f, 0.0f, 0.0f };
	mNetForce = { 0.0f, 0.0f, 0.0f };
	mVelocity = { 0.0f, 0.0f, 0.0f };
	mAcceleration = { 0.0f, 0.0f, 0.0f };
}

void PhysicsModel::Update( const float dt )
{
	AddWeight();
	CalculateAcceleration();
	AddFriction( dt );
	CalculateVelocity( dt );
	AddDrag();
	CalculatePosition( dt );
	AddGravity();

	mNetForce = { 0.0f, 0.0f, 0.0f };
}

void PhysicsModel::AddWeight()
{
	mNetForce.y -= mWeight;
}

void PhysicsModel::CalculateAcceleration()
{
	mAcceleration.x = mNetForce.x / mMass;
	mAcceleration.y = mNetForce.y / mMass;
	mAcceleration.z = mNetForce.z / mMass;
}

void PhysicsModel::CalculateVelocity( const float dt )
{
	mVelocity.x += mAcceleration.x * dt;
	mVelocity.y += mAcceleration.y * dt;
	mVelocity.z += mAcceleration.z * dt;
}

void PhysicsModel::AddFriction( const float dt )
{
	// f = u * N
	XMFLOAT3 invVelocity = { -mVelocity.x, -mVelocity.y, -mVelocity.z };
	if ( Magnitude( mVelocity ) < mFrictionFactor * dt )
	{
		mFriction.x = invVelocity.x / dt;
		mFriction.y = invVelocity.y / dt;
		mFriction.z = invVelocity.z / dt;
	}
	else
	{
		mFriction.x = Normalization( invVelocity ).x * mFrictionFactor;
		mFriction.y = Normalization( invVelocity ).y * mFrictionFactor;
		mFriction.z = Normalization( invVelocity ).z * mFrictionFactor;
	}
}

void PhysicsModel::AddDrag()
{
	mUseLaminar ? LaminarDrag() : TurbulentDrag();
}

void PhysicsModel::LaminarDrag()
{
	mDrag.x = mDragFactor * mVelocity.x;
	mDrag.y = mDragFactor * mVelocity.y;
	mDrag.z = mDragFactor * mVelocity.z;
}

void PhysicsModel::TurbulentDrag()
{
	float magVelocity = Magnitude( mVelocity );
	XMFLOAT3 unitVelocity = Normalization( mVelocity );

	float dragMagnitude = mDragFactor * magVelocity * magVelocity;

	mDrag.x = -dragMagnitude * mDragFactor * unitVelocity.x;
	mDrag.y = -dragMagnitude * mDragFactor * unitVelocity.y;
	mDrag.z = -dragMagnitude * mDragFactor * unitVelocity.z;
}

void PhysicsModel::CalculatePosition( const float dt )
{
	mPosition = mTransform->GetPositionFloat3();
	mPosition.x += mVelocity.x * dt + 0.5f * mAcceleration.x * dt * dt;
	mPosition.y += mVelocity.y * dt + 0.5f * mAcceleration.y * dt * dt;
	mPosition.z += mVelocity.z * dt + 0.5f * mAcceleration.z * dt * dt;

	CalculateVelocity( dt );
	mTransform->SetPosition( mPosition );
}

void PhysicsModel::AddGravity()
{
	mPosition = mTransform->GetPositionFloat3();

	if ( mPosition.y < 1.0f )
	{
		mVelocity = { mVelocity.x, 0.0f, mVelocity.z };
		mPosition.y = 1.0f;
		mTransform->SetPosition( mPosition );
	}
	else if (mPosition.y > 1.0f)
	{
		mVelocity.y -= 0.01f;
	}
}

XMFLOAT3 PhysicsModel::Normalization( XMFLOAT3 vec ) const noexcept
{
	XMFLOAT3 temp = vec;
	float unitVector = sqrt( ( temp.x * temp.x ) + ( temp.y * temp.y ) + ( temp.z * temp.z ) );

	temp.x = temp.x / unitVector;
	temp.y = temp.y / unitVector;
	temp.z = temp.z / unitVector;

	return temp;
}