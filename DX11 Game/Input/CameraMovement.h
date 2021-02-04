#pragma once
#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

#include "Camera.h"

class CameraMovement
{
public:
	static void MoveForward( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetForwardVector() * cam->GetCameraSpeed() * dt );
	}

	static void MoveBackward( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetBackwardVector() * cam->GetCameraSpeed() * dt );
	}

	static void MoveLeft( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetLeftVector() * cam->GetCameraSpeed() * dt );
	}

	static void MoveRight( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetRightVector() * cam->GetCameraSpeed() * dt );
	}

	static void MoveUp( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetUpVector() * cam->GetCameraSpeed() * dt );
	}

	static void MoveDown( std::unique_ptr<Camera>& cam, const float dt )
	{
		cam->AdjustPosition( cam->GetUpVector() * -cam->GetCameraSpeed() * dt );
	}
};

#endif