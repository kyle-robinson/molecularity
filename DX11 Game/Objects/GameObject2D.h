#pragma once
#ifndef GAMEOBJECT2D_H
#define GAMEOBJECT2D_H

#include "GameObject.h"

class GameObject2D : public GameObject
{
protected:
	virtual void UpdateMatrix();
};

#endif