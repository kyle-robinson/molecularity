#pragma once

enum ToolType {CONVERT, RESIZE};


/// <summary>
/// Editable object properties by the multitool. No CPP file at the moment since once was not needed.
/// </summary>

class CubeProperties
{
public:
	
	bool GetBurnable() { return isBurnable; }
	void SetBurnable(bool burnable) { isBurnable = burnable; }

	bool GetReflective() { return isReflective; }
	void SetReflective(bool reflective) { isReflective = reflective; }

	ToolType GetType() { return toolType; }
	void SetType(ToolType type) { toolType = type; }

	float GetSizeMultiplier() { return sizeMultiplier; }
	void SetSizeMultiplier(float multiplier) { sizeMultiplier = multiplier; }



private:
	bool isBurnable = false;
	bool isReflective = false;
	ToolType toolType = CONVERT;
	float sizeMultiplier;

};