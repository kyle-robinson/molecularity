#pragma once
#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

#include "Camera.h"

class CameraMovement
{
public:
	static void MoveForward( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetForwardVector() * cam.GetCameraSpeed() * dt );
	}

	static void MoveBackward( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetBackwardVector() * cam.GetCameraSpeed() * dt );
	}

	static void MoveLeft( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetLeftVector() * cam.GetCameraSpeed() * dt );
	}

	static void MoveRight( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetRightVector() * cam.GetCameraSpeed() * dt );
	}

	static void MoveUp( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetUpVector() * cam.GetCameraSpeed() * dt );
	}

	static void MoveDown( Camera& cam, const float dt )
	{
		cam.AdjustPosition( cam.GetUpVector() * -cam.GetCameraSpeed() * dt );
	}
};

#endif