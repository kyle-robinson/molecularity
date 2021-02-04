#pragma once
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Camera.h"
#include "GameObject3D.h"

class Billboard
{
public:
	static float BillboardModel( std::unique_ptr<Camera>& cam, GameObject& obj )
	{
		double angle = atan2(
			obj.GetPositionFloat3().x - cam->GetPositionFloat3().x,
			obj.GetPositionFloat3().z - cam->GetPositionFloat3().z )
			* ( 180.0 / XM_PI );
		return static_cast<float>( angle ) * 0.0174532925f;
	}
};

#endif