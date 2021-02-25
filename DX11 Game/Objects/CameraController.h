#pragma once
#include "Camera2D.h"
#include "Camera.h"
#include "JSON_Helper.h""
/// <summary>
/// Handles the user's current camera
/// </summary>
class CameraController
{
public:
	void Initialize(int width, int height);


	std::unique_ptr<Camera>& GetCamera(const JSON::CameraType& cam) noexcept { return cameras[cam]; } //Gets a specific camera
	
	void SetCurrentCamera(JSON::CameraType cam) { cameraToUse = cam; } //Sets CurrentCamera
	JSON::CameraType GetCurrentCamera() { return cameraToUse; } //Gets Current Camera

	Camera2D GetUICamera() { return UICamera; }

private:


private:
	std::map<JSON::CameraType, std::unique_ptr<Camera>> cameras;
	JSON::CameraType cameraToUse = JSON::CameraType::Default;
	Camera2D UICamera;




};

