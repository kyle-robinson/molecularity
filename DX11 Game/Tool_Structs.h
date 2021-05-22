#pragma once
/// <summary>
/// Data to do with tool
/// </summary>
enum class ToolType
{
	Convert,
	Resize,
	Magnetism
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


enum class MagnetismMode
{
	AllCubes=0,
	OneCube
};

struct ToolData
{
	BoxType boxtype;
	BoxSize boxSize;
	MagnetismMode MagMode;
};