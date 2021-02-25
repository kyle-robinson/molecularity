#pragma once
#ifndef CUBEPROPERTIES_H
#define CUBEPROPERTIES_H

/// <summary>
/// Editable object properties by the multitool. No CPP file at the moment since once was not needed.
/// </summary>

static enum class ToolType
{
	Convert,
	Resize
};

static enum class BoxType
{
	Default,
	Bounce,
	Arrow,
	TNT
};

static enum class BoxSize
{
	Small,
	Normal,
	Large
};

class CubeProperties
{
public:
	// Getters	
	ToolType GetToolType() const noexcept { return toolType; }
	BoxType GetBoxType() const noexcept { return boxType; }
	BoxSize GetBoxSize() const noexcept { return boxSize; }
	int GetBoxMaterialID() const noexcept { return boxMaterialID; }

	bool GetBurnable() const noexcept { return isBurnable; }
	bool GetReflective() const noexcept { return isReflective; }
	float GetSizeMultiplier() const noexcept { return sizeMultiplier; }

	// Setters
	void SetToolType( ToolType type ) { toolType = type; }
	void SetBoxType( BoxType type ) noexcept { boxType = type; }
	void SetBoxSize( BoxSize size ) noexcept { boxSize = size; }
	void SetBoxMaterialID( int boxID ) noexcept { boxMaterialID = boxID; }

	void SetBurnable( bool burnable ) noexcept { isBurnable = burnable; }
	void SetReflective( bool reflective ) noexcept { isReflective = reflective; }
	void SetSizeMultiplier( float multiplier ) noexcept { sizeMultiplier = multiplier; }

private:
	int boxMaterialID = 0;
	bool isBurnable = false;
	bool isReflective = false;
	float sizeMultiplier = 1.0f;

	BoxSize boxSize = BoxSize::Normal;
	BoxType boxType = BoxType::Default;
	ToolType toolType = ToolType::Convert;
};

#endif