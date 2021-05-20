#pragma once
/// <summary>
/// Data to do with tool
/// </summary>
enum class ToolType
{
	Convert,
	Resize
};

enum class BoxType
{
	Mesh = 0,
	Wood,
	Stone,
	Iron,
	Alien
};
enum class BoxSize
{
	Small = 0,
	Normal,
	Large
};


struct ToolData
{
	BoxType boxtype;
	BoxSize boxSize;
};