#pragma once
#ifndef COLLISIONS_H
#define COLLISIONS_H

class Cube;
class Camera;
class GameObject3D;
#include <memory>

/// <summary>
/// Provides a list of static functions for checking collisions between the target camera and a game objects.
/// Currently only defines simple collisions, used to keep the player contained within the play area.
/// </summary>
class Collisions
{
public:
	static bool CheckCollisionCircle( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept;
	static bool CheckCollisionSphere( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept;
	
	static void CeilingCollision(std::shared_ptr<Cube>& cube, float ceilingHeight ) noexcept;

	static void CheckCollisionLevel1( std::unique_ptr<Camera>& camera, float offset ) noexcept;
	static void CheckCollisionLevel1( std::shared_ptr<Cube>& cube, float offset ) noexcept;

	static void CheckCollisionLevel2( std::unique_ptr<Camera>& camera, float offset ) noexcept;
	static void CheckCollisionLevel2( std::shared_ptr<Cube>& cube, float offset ) noexcept;
};

#endif