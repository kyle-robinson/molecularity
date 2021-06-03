#pragma once
#ifndef CUBEPROPERTIES_H
#define CUBEPROPERTIES_H

#include "stdafx.h"
#include "Tool_Structs.h"

/// <summary>
/// Editable object properties by the multitool.
/// </summary>
class CubeProperties
{
public:
	// Getters	
	BoxType GetBoxType() const noexcept { return boxType; }
	BoxSize GetBoxSize() const noexcept { return boxSize; }
	BoxBounce GetBoxBounce() const noexcept { return boxBounce; }

	int GetSizeID() const noexcept { return sizeID; }
	int GetMaterialID() const noexcept { return materialID; }

	bool GetBurnable() const noexcept { return isBurnable; }
	bool GetReflective() const noexcept { return isReflective; }
	float GetSizeMultiplier() const noexcept { return sizeMultiplier; }
	bool GetBoxMagneticMove() { return isMagneticMove; }

	XMFLOAT3 GetOutlineColor() const noexcept { return outlineColor; }
	bool GetConductivity() const noexcept { return mConductive; }
	bool GetIsMagnetic() const noexcept { return isMagnetic; }
	
	// Setters
	void SetBoxType( BoxType type ) noexcept { boxType = type; }
	void SetBoxSize( BoxSize size ) noexcept
	{
		boxSize = size;
		if ( boxSize == BoxSize::Small )
			SetSizeMultiplier( 0.5f );
		else if ( boxSize == BoxSize::Normal )
			SetSizeMultiplier( 1.0f );
		else if ( boxSize == BoxSize::Large )
			SetSizeMultiplier( 2.0f );
	}

	void SetMagnetic( bool mag )noexcept { isMagnetic = mag; }
	void SetBoxMagneticMove( bool isMove ) noexcept { isMagneticMove = isMove; }
	void SetBoxBounce( BoxBounce bouncy ) noexcept { boxBounce = bouncy; }
	void SetConductive( bool conductive ) noexcept { mConductive = conductive; }
	void SetSizeID( int id ) noexcept { sizeID = id; }
	void SetMaterialID( int id ) noexcept { materialID = id; }

	void SetBurnable( bool burnable ) noexcept { isBurnable = burnable; }
	void SetReflective( bool reflective ) noexcept { isReflective = reflective; }
	void SetSizeMultiplier( float multiplier ) noexcept { sizeMultiplier = multiplier; }

	void SetOutlineColor( XMFLOAT3 color ) noexcept { outlineColor = color; }
private:
	int sizeID = 1;
	int materialID = 0;
	bool isBurnable = false;
	bool isReflective = false;
	bool isMagnetic = false;
	bool isMagneticMove = false;
	bool mConductive = false;
	float sizeMultiplier = 1.0f;
	XMFLOAT3 outlineColor = { 0.0f, 0.0f, 0.0f };

	BoxType boxType = BoxType::DissCube;
	BoxSize boxSize = BoxSize::Normal;
	BoxBounce boxBounce = BoxBounce::Solid;
};

#endif