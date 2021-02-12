#pragma once
#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <memory>
class Camera;
class GameObject3D;

class Collisions
{
public:
	static bool CheckCollisionCircle( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept;
};

#endif