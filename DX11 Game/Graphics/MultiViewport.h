#pragma once
#ifndef MULTIVIEWPORT_H
#define MULTIVIEWPORT_H

class LevelContainer;

/// <summary>
/// Handles the camera view on a given viewport.
/// Binds a viewport to be rendered with the appropriate camera view.
/// </summary>
class MultiViewport
{
public:
	MultiViewport() = default;
	void Update( LevelContainer& manager );
	bool IsUsingSub() const noexcept { return useViewportSub; }
	bool IsUsingMain() const noexcept { return useViewportMain; }
	void SetUsingSub( bool sub = true ) noexcept { useViewportSub = sub; }
	void SetUsingMain( bool main = true ) noexcept { useViewportMain = main; }
private:
	bool useViewportSub = false;
	bool useViewportMain = false;
};

#endif