#pragma once
/// <summary>
/// Data to do with tool
/// </summary>
enum class ToolType
{
	Convert,
	Resize,
	Bounce
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

enum class BoxBounce
{
	Solid,
	Bouncy
};

struct ToolData
{
	BoxType boxtype;
	BoxSize boxSize;
	BoxBounce boxBounce;
};