#include "stdafx.h"
#include "CameraController.h"

void CameraController::Initialize(int width, int height)
{
	// CAMERAS
	{
		XMFLOAT2 aspectRatio = { static_cast<float>(width), static_cast<float>(height) };
		cameras.emplace(JSON::CameraType::Default, std::make_unique<Camera>(0.0f, 9.0f, -15.0f));
		cameras.emplace(JSON::CameraType::Static, std::make_unique<Camera>(0.0f, 9.0f, 0.0f));
		cameras.emplace(JSON::CameraType::Debug, std::make_unique<Camera>(0.0f, 9.0f, -10.0f));
		for (const auto& cam : cameras)
			cam.second->SetProjectionValues(70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f);
		UICamera.SetProjectionValues(aspectRatio.x, aspectRatio.y, 0.0f, 1.0f);
	}

}