#pragma once
#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

#include "Camera.h"

class CameraMovement
{
public:
	static void MoveForward( std::unique_ptr<Camera>& cam, const bool playMode, const float dt )
	{
		cam->AdjustPosition( cam->GetForwardVector( playMode ) * cam->GetCameraSpeed() * dt );
	}

	static void MoveBackward( std::unique_ptr<Camera>& cam, const bool playMode, const float dt )
	{
		cam->AdjustPosition( cam->GetBackwardVector( playMode ) * cam->GetCameraSpeed() * dt );
	}

	static void MoveLeft( std::unique_ptr<Camera>& cam, const bool playMode, const float dt )
	{
		cam->AdjustPosition( cam->GetLeftVector( playMode ) * cam->GetCameraSpeed() * dt );
	}

	static void MoveRight( std::unique_ptr<Camera>& cam, const bool playMode, const float dt )
	{
		cam->AdjustPosition( cam->GetRightVector( playMode ) * cam->GetCameraSpeed() * dt );
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