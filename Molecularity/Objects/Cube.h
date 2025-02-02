#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "RenderableGameObject.h"
#include "CubeProperties.h"
#include "PhysicsModel.h"

/// <summary>
/// Create a simple cube object.
/// Inherits from RenderableGameObject which allows for the individual setting of position/rotation/scale data.
/// </summary>
class Cube : public RenderableGameObject
{
public:
	bool Initialize( ID3D11DeviceContext* context, ID3D11Device* device );
	void Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ID3D11ShaderResourceView* texture ) noexcept;
	void CheckCollisionAABB( std::shared_ptr<Cube>& object, const float dt ) noexcept;
	bool CheckCollisionAABB( RenderableGameObject& object, const float dt ) noexcept;
	
	// Getters
	bool GetIsHolding() const noexcept { return isHeld; }
	bool GetIsHovering() const noexcept { return cubeHover; }
	bool GetIsInRange() const noexcept { return cubeInRange; }
	bool GetIsDissCube() const noexcept { return isDissCube; }

	// Setters
	void SetIsHolding( bool isHolding ) noexcept { isHeld = isHolding; }
	void SetIsHovering( bool hover ) noexcept { cubeHover = hover; }
	void SetIsInRange( bool range ) noexcept { cubeInRange = range; }
	void SetCamPos( XMFLOAT3 newcampos ) noexcept { CamPos = newcampos; }
	void SetIsDissCube( bool isDiss ) noexcept { isDissCube = isDiss; }

	void Update( const float deltaTime ) noexcept;
	std::shared_ptr<PhysicsModel> GetPhysicsModel() const noexcept { return physicsModel; }
	std::shared_ptr<CubeProperties> GetEditableProperties() const noexcept { return editableProperties; }
private:
	void CollisionResolution( std::shared_ptr<Cube>& object, const float dt ) noexcept;
	void MagneticForce();
	std::shared_ptr<CubeProperties> editableProperties;
	std::shared_ptr<PhysicsModel> physicsModel;
	ID3D11DeviceContext* context;
	VertexBuffer<Vertex3D> vb_cube;
	IndexBuffer ib_cube;

	bool isHeld = false;
	bool heldLastFrame = false;
	bool cubeHover = false;
	bool cubeInRange = false;
	bool isDissCube = true;

	int delay;
	XMFLOAT3 pos;
	XMFLOAT3 prevPos;

	// Magnetic data
	XMFLOAT3 CamPos;
	float MagPower = 5;
};

#endif