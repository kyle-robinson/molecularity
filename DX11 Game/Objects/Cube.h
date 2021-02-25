#pragma once
#ifndef CUBE_H
#define CUBE_H

/// <summary>
/// Create a simple cube object.
/// Inherits from RenderableGameObject which allows for the individual setting of position/rotation/scale data.
/// </summary>

#include "RenderableGameObject.h"
#include "CubeProperties.h"
#include "PhysicsModel.h"

static enum class BoxType
{
	Default,
	Bounce,
	Arrow,
	TNT
};

class Cube : public RenderableGameObject
{
public:
	bool Initialize( ID3D11DeviceContext* context, ID3D11Device* device );
	void Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ID3D11ShaderResourceView* texture ) noexcept;
	
	// Getters
	bool GetIsHolding() const noexcept { return isHeld; }
	bool GetIsHovering() const noexcept { return cubeHover; }
	bool GetIsInRange() const noexcept { return cubeInRange; }
	int GetBoxToUse() const noexcept { return boxToUse; }
	int GetSizeAmount() const noexcept { return sizeAmount; }
	float GetSizeToUse() const noexcept { return sizeToUse; }
	BoxType GetBoxType() const noexcept { return selectedBox; }

	// Setters
	void SetIsHolding( bool isHolding ) noexcept { isHeld = isHolding; }
	void SetIsHovering( bool hover ) noexcept { cubeHover = hover; }
	void SetIsInRange( bool range ) noexcept { cubeInRange = range; }
	void SetBoxToUse( int box ) noexcept { boxToUse = box; }
	void SetSizeAmount( int size ) noexcept { sizeAmount = size; }
	void SetSizeToUse( float newSize ) noexcept { sizeToUse = newSize; }
	void SetBoxType( BoxType boxType ) noexcept { selectedBox = boxType; }

	void UpdatePhysics( const float deltaTime ) noexcept;
	std::shared_ptr<CubeProperties> GetEditableProperties() const noexcept { return editableProperties; }
private:
	std::shared_ptr<CubeProperties> editableProperties;
	std::shared_ptr<PhysicsModel> physicsModel;
	ID3D11DeviceContext* context;
	VertexBuffer<Vertex3D> vb_cube;
	IndexBuffer ib_cube;
	
	int boxToUse = 0;
	int sizeAmount = 2;
	float sizeToUse = 1.0f;

	bool isHeld = false;
	bool cubeHover = false;
	bool cubeInRange = false;

	//std::string selectedBox = "Basic";
	BoxType selectedBox = BoxType::Default;
};

#endif