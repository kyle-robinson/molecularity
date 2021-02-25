#pragma once
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Camera2D.h"
#include "Camera.h"
#include "JSON_Helper.h""

/// <summary>
/// Handles the user's current camera.
/// </summary>
class CameraController
{
public:
	void Initialize( int width, int height );
	void Update();

	// Get a handle to specific camera
	std::unique_ptr<Camera>& GetCamera( const JSON::CameraType& cam ) noexcept { return cameras[cam]; }

	// Sets active camera
	void SetCurrentCamera( JSON::CameraType cam ) noexcept { cameraToUse = cam; }

	// Gets current active camera
	JSON::CameraType GetCurrentCamera() const noexcept { return cameraToUse; }

	Camera2D GetUICamera() const noexcept { return UICamera; }
private:
	Camera2D UICamera;
	JSON::CameraType cameraToUse = JSON::CameraType::Default;
	std::map<JSON::CameraType, std::unique_ptr<Camera>> cameras;
};

#endif