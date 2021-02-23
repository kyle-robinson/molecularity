#pragma once
#ifndef COLLISIONS_H
#define COLLISIONS_H

/// <summary>
/// Provides a list of static functions for checking collisions between the target camera and a game objects.
/// Currently only defines simple collisions, used to keep the player contained within the play area.
/// </summary>

#include <memory>
class Camera;
class GameObject3D;

class Collisions
{
public:
	static bool CheckCollisionCircle( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept;
	static bool CheckCollisionSphere( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept;
};

#endif