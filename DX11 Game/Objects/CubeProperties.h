#pragma once
#ifndef CUBEPROPERTIES_H
#define CUBEPROPERTIES_H

/// <summary>
/// Editable object properties by the multitool. No CPP file at the moment since once was not needed.
/// </summary>

enum class ToolType { CONVERT, RESIZE };

class CubeProperties
{
public:
	bool GetBurnable() const noexcept { return isBurnable; }
	void SetBurnable( bool burnable ) noexcept { isBurnable = burnable; }

	bool GetReflective() const noexcept { return isReflective; }
	void SetReflective( bool reflective ) noexcept { isReflective = reflective; }

	ToolType GetType() const noexcept { return toolType; }
	void SetType( ToolType type ) { toolType = type; }

	float GetSizeMultiplier() const noexcept { return sizeMultiplier; }
	void SetSizeMultiplier( float multiplier ) noexcept { sizeMultiplier = multiplier; }

private:
	float sizeMultiplier;
	bool isBurnable = false;
	bool isReflective = false;
	ToolType toolType = ToolType::CONVERT;
};

#endif