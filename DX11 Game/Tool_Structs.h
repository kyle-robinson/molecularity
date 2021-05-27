#pragma once
/// <summary>
/// Data to do with tool
/// </summary>
enum class ToolType
{
	Convert,
	Resize,
	Magnetism,
	Bounce
};

enum class BoxType
{
	Mesh = 0,
	Wood,
	Iron,
	DissCube
};

enum class BoxSize
{
	Small = 0,
	Normal,
	Large
};

enum class BoxBounce
{
	Solid,
	Bouncy
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
	BoxBounce boxBounce;
};