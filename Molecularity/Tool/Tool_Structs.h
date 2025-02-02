#pragma once
#ifndef TOOL_STRUCTS_H
#define TOOL_STRUCTS_H

/// <summary>
/// Data relating to the multi-tool.
/// </summary>

enum class ToolType
{
	Convert,
	Resize,
	Magnetism,
	Bounce,
	Conductive
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
	AllCubes = 0,
	OneCube
};

enum class BoxConductive
{
	NotConductive,
	IsConductive
};

struct ToolData
{
	BoxType boxtype;
	BoxSize boxSize;
	MagnetismMode MagMode;
	BoxBounce boxBounce;
	BoxConductive boxConductive;
};

#endif