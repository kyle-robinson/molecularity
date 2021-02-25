#pragma once
#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

#include "GameObject.h"

/// <summary>
/// 2D game objects must inherit from this class.
/// Inherits from GameObject to allow for modification of position/rotation/scale data.
/// </summary>
class GameObject2D : public GameObject
{
protected:
	virtual void UpdateMatrix();
};

#endif