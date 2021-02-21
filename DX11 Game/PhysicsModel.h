#pragma once
#include "Objects/RenderableGameObject.h"

class PhysicsModel
{
public:
	PhysicsModel(RenderableGameObject* transform);
	~PhysicsModel();

	//get/set forces
	XMFLOAT3 GetVelocity() const { return mVelocity; }
	void SetVelocity(XMFLOAT3 velocity) { mVelocity = velocity; }

	XMFLOAT3 GetAcceleration() const { return mAcceleration; }
	void SetAcceleration(XMFLOAT3 acceleration) { mAcceleration = acceleration; }

	//functions for updating forces and object position
	virtual void Update(const float deltaTime);
	void AddWeight();
	void CalculateAcceleration();
	void CalculateVelocity(const float deltaTime);
	void AddFriction(const float deltaTime);
	void AddDrag();
	void LaminarDrag();
	void TurbulentDrag();
	void CalculatePosition(const float deltaTime);
	void AddGravity();

	float Magnitude(XMFLOAT3 vec) const { return (sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2))); }
	XMFLOAT3 Normalization(XMFLOAT3 vec) const;
private:
	//constants
	float mGravity = 9.81f;
	float mDragFactor = 0.75f;

	//local property variables
	RenderableGameObject* mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mNetForce;
	XMFLOAT3 mVelocity;
	XMFLOAT3 mAcceleration;
	XMFLOAT3 mDrag;
	float mMass;
	float mWeight;
	bool mUseLaminar;
};