#include "stdafx.h"
#include "CameraController.h"

void CameraController::Initialize( int width, int height )
{
	// CAMERAS
	{
		XMFLOAT2 aspectRatio = { static_cast<float>( width ), static_cast<float>( height ) };
		cameras.emplace( JSON::CameraType::Default, std::make_unique<Camera>( 0.0f, 9.0f, -15.0f ) );
		cameras.emplace( JSON::CameraType::Static, std::make_unique<Camera>( 0.0f, 9.0f, 0.0f ) );
		cameras.emplace( JSON::CameraType::Debug, std::make_unique<Camera>( 0.0f, 9.0f, -10.0f ) );
		for ( const auto& cam : cameras )
			cam.second->SetProjectionValues( 70.0f, aspectRatio.x / aspectRatio.y, 0.1f, 1000.0f );
		UICamera.SetProjectionValues( aspectRatio.x, aspectRatio.y, 0.0f, 1.0f );
	}
}

void CameraController::Update()
{
	//Currently forms nothing of value since cameras arent doing anything by themselves. 
	//In the future will need cameras to passively connect themselves to the player (default) which can be done here / in the player
	UICamera.SendWorldOrthoMatrix();
}

void CameraController::CollisionResolution( std::unique_ptr<Camera>& camera, GameObject& world, const float dt ) noexcept
{
	float dx = world.GetPositionFloat3().x - camera->GetPositionFloat3().x;
	float dz = world.GetPositionFloat3().z - camera->GetPositionFloat3().z;
	float length = std::sqrtf( dx * dx + dz * dz );
	dx /= length;
	dz /= length;
	dx *= camera->GetCameraSpeed() * dt;
	dz *= camera->GetCameraSpeed() * dt;
	camera->AdjustPosition( dx, 0.0f, dz );
}

void CameraController::AddToEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
}

void CameraController::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
		case EVENTID::WindowSizeChangeEvent:
			XMFLOAT2 _SizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());
			UICamera.SetProjectionValues(_SizeOfScreen.x, _SizeOfScreen.y, 0.0f, 1.0f);
			for (const auto& cam : cameras)
				cam.second->SetProjectionValues(70.0f, _SizeOfScreen.x / _SizeOfScreen.y, 0.1f, 1000.0f);
			break;

	}
}
