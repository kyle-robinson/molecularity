#pragma once
#include<Utility/stdafx.h>
#ifndef CUBEPROPERTIES_H
#define CUBEPROPERTIES_H

enum class ToolType
{
	Convert,
	Resize
};

enum class BoxType
{
	Mesh,
	Wood,
	Stone,
	Iron,
	Alien
};

enum class BoxSize
{
	Small,
	Normal,
	Large
};

/// <summary>
/// Editable object properties by the multitool.
/// </summary>
class CubeProperties
{
public:
	// Getters	
	ToolType GetToolType() const noexcept { return toolType; }
	BoxType GetBoxType() const noexcept { return boxType; }
	BoxSize GetBoxSize() const noexcept { return boxSize; }
	
	int GetSizeID() const noexcept { return sizeID; }
	int GetMaterialID() const noexcept { return materialID; }

	bool GetBurnable() const noexcept { return isBurnable; }
	bool GetReflective() const noexcept { return isReflective; }
	float GetSizeMultiplier() const noexcept { return sizeMultiplier; }

	// Setters
	void SetToolType( ToolType type ) { toolType = type;}
	void SetBoxType( BoxType type ) noexcept { boxType = type; }
	void SetBoxSize( BoxSize size ) noexcept { boxSize = size;}
	
	void SetSizeID( int id ) noexcept { sizeID = id; }
	void SetMaterialID( int id ) noexcept { materialID = id; }

	void SetBurnable( bool burnable ) noexcept { isBurnable = burnable; }
	void SetReflective( bool reflective ) noexcept { isReflective = reflective; }
	void SetSizeMultiplier( float multiplier ) noexcept { sizeMultiplier = multiplier;}

private:
	int sizeID = 1;
	int materialID = 0;
	bool isBurnable = false;
	bool isReflective = false;
	float sizeMultiplier = 1.0f;
	BoxType boxType = BoxType::Wood;
	BoxSize boxSize = BoxSize::Normal;
	ToolType toolType = ToolType::Convert;
};

#endif