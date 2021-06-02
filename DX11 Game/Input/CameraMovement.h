#pragma once
#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

#include "Camera.h"

/// <summary>
/// Provides a list of static functions to update camera movement.
/// If 'playMode' is false, then debug mode is in use and Y movement vectors are used for fly camera, otherwise they are omitted.
/// </summary>
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

	static void Jump( std::unique_ptr<Camera>& cam, bool& jumping, const float dt )
	{
		static int maxValue = 25;
		static int counter = maxValue;
		jumping = true;

		if ( counter > 0 )
		{
			cam->AdjustPosition( cam->GetUpVector() * cam->GetCameraSpeed() * dt );
		}
		else if ( counter < 0 && counter > -maxValue )
		{
			cam->AdjustPosition( cam->GetUpVector() * -cam->GetCameraSpeed() * dt );
		}

		if ( counter < -maxValue )
		{
			jumping = false;
			counter = maxValue;
		}

		counter--;
	}
};

#endif