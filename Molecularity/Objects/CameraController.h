#pragma once
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Camera2D.h"
#include "Camera.h"
#include "JSON_Helper.h""

/// <summary>
/// Handles the user's current camera.
/// </summary>
class CameraController : public Listener
{
public:
	void Initialize( int width, int height );
	void Update();

	// Get a handle to a specific camera
	std::unique_ptr<Camera>& GetCamera( const JSON::CameraType& cam ) noexcept { return cameras[cam]; }

	// Sets active camera
	void SetCurrentCamera( JSON::CameraType cam ) noexcept { cameraToUse = cam; }
	void SetIsUsingMain( bool cam ) noexcept { usingMainCamera = cam; }
	bool GetIsUsingMain() const noexcept { return usingMainCamera; }

	// Gets current active camera
	JSON::CameraType GetCurrentCamera() const noexcept { return cameraToUse; }

	Camera2D GetUICamera() const noexcept { return UICamera; }

	void CollisionResolution( std::unique_ptr<Camera>& camera, GameObject& world, const float dt ) noexcept;

	void AddToEvent();
	void HandleEvent( Event* event );
private:
	Camera2D UICamera;
	bool usingMainCamera = true;
	JSON::CameraType cameraToUse = JSON::CameraType::Default;
	std::unordered_map<JSON::CameraType, std::unique_ptr<Camera>> cameras;
};

#endif