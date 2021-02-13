#include "Application.h"
#include "CameraMovement.h"

bool Application::Initialize(
	HINSTANCE hInstance,
	const std::string& windowTitle,
	const std::string& windowClass,
	int width,
	int height )
{
	timer.Start();
	EnableCursor();

	if ( !renderWindow.Initialize( this, hInstance, windowTitle, windowClass, width, height ) )
		return false;

	if ( !gfx.Initialize( renderWindow.GetHWND(), width, height ) )
		return false;

	mousePick.Initialize( gfx.cameras[gfx.cameraToUse]->GetViewMatrix(),
		gfx.cameras[gfx.cameraToUse]->GetProjectionMatrix(), width, height );

	return true;
}

bool Application::ProcessMessages() noexcept
{
	return renderWindow.ProcessMessages();
}

void Application::Update()
{
	float dt = timer.GetMilliSecondsElapsed();
	timer.Restart();

	// Read Input
    while ( !keyboard.CharBufferIsEmpty() )
	{
		unsigned char ch = keyboard.ReadChar();
	}
	while ( !keyboard.KeyBufferIsEmpty() )
	{
		Keyboard::KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}
	while ( !mouse.EventBufferIsEmpty() )
	{
		Mouse::MouseEvent me = mouse.ReadEvent();

		// Camera Orientation
		if ( mouse.IsRightDown() )
		{
			if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
			{
				gfx.cameras[gfx.cameraToUse]->AdjustRotation(
					XMFLOAT3(
						static_cast<float>( me.GetPosY() ) * 0.005f,
						static_cast<float>( me.GetPosX() ) * 0.005f,
						0.0f
					)
				);
			}
		}
		
		// Mouse Picking
		mousePick.UpdateMatrices( gfx.cameras[gfx.cameraToUse]->GetViewMatrix(),
			gfx.cameras[gfx.cameraToUse]->GetProjectionMatrix() );
		if ( mousePick.TestIntersection( me.GetPosX(), me.GetPosY(), *gfx.cube.get() ) )
			gfx.cubeHover = true;
		else
			gfx.cubeHover = false;
		
		// Manage Multi-Tool Options
		if ( gfx.toolType == gfx.CONVERT )
		{
			// Change selected texture to use on box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && gfx.boxToUse < 3 )
				gfx.boxToUse++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && gfx.boxToUse > 0 )
				gfx.boxToUse--;

			// Update box texture on click while hovering
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && gfx.cubeHover )
			{
				switch ( gfx.boxToUse )
				{
				case 0: gfx.selectedBox = "Default"; break;
				case 1: gfx.selectedBox = "Bounce"; break;
				case 2: gfx.selectedBox = "Jump"; break;
				case 3: gfx.selectedBox = "TNT"; break;
				}
			}
		}
		else if ( gfx.toolType == gfx.RESIZE )
		{
			// Change size amount to change box to
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && gfx.sizeAmount < 2 )
				gfx.sizeAmount++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && gfx.sizeAmount > 0 )
				gfx.sizeAmount--;

			// Set the box scale to use based on the option previously selected
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && gfx.cubeHover )
			{
				switch ( gfx.sizeAmount )
				{
				case 0: gfx.sizeToUse = 0.25f; break;
				case 1: gfx.sizeToUse = 1.0f; break;
				case 2: gfx.sizeToUse = 2.0f; break;
				}
			}
		}
	}

	// Set Camera To Use
	if ( keyboard.KeyIsPressed( VK_F1 ) ) gfx.cameraToUse = "Default";
	if ( keyboard.KeyIsPressed( VK_F2 ) ) gfx.cameraToUse = "Static";
	if ( keyboard.KeyIsPressed( VK_F3 ) ) gfx.cameraToUse = "Debug";

	// Camera Movement
	if ( gfx.cameraToUse != "Static" )
	{
		gfx.cameras[gfx.cameraToUse]->SetCameraSpeed( 0.002f );
		if ( keyboard.KeyIsPressed( VK_SHIFT ) ) gfx.cameras[gfx.cameraToUse]->SetCameraSpeed( 0.01f );
		if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( gfx.cameras[gfx.cameraToUse], dt );
	}
	if ( gfx.cameraToUse == "Debug" )
	{
		if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( gfx.cameras["Debug"], dt );
		if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( gfx.cameras["Debug"], dt );
	}

	// Camera World Collisions
	if ( !gfx.wallCollision )
	{
		float dx = gfx.hubRoom.GetPositionFloat3().x - gfx.cameras["Default"]->GetPositionFloat3().x;
		float dz = gfx.hubRoom.GetPositionFloat3().z - gfx.cameras["Default"]->GetPositionFloat3().z;
		float length = std::sqrtf( dx * dx + dz * dz );
		dx /= length;
		dz /= length;
		dx *= gfx.cameras["Default"]->GetCameraSpeed() * 10.0f;
		dz *= gfx.cameras["Default"]->GetCameraSpeed() * 10.0f;
		gfx.cameras["Default"]->AdjustPosition( dx, 0.0f, dz );
	}

	// Prevent Camera Y-Axis Movement
	gfx.cameras["Default"]->SetPosition(
		gfx.cameras["Default"]->GetPositionFloat3().x,
		9.0f,
		gfx.cameras["Default"]->GetPositionFloat3().z
	);

	// Multi-Tool Type
	if ( keyboard.KeyIsPressed( '1' ) ) gfx.toolType = gfx.CONVERT;
	if ( keyboard.KeyIsPressed( '2' ) ) gfx.toolType = gfx.RESIZE;

	// Pick-Up Cube - Set position relative to camera
	if ( keyboard.KeyIsPressed( 'E' ) && gfx.cameraToUse != "Static" )
	{
		XMVECTOR cubePosition = gfx.cameras[gfx.cameraToUse]->GetPositionVector();
		cubePosition += gfx.cameras[gfx.cameraToUse]->GetForwardVector() * 2;
		gfx.cube->SetPosition( cubePosition );
		gfx.cube->SetRotation(
			gfx.cube->GetRotationFloat3().x,
			gfx.cameras[gfx.cameraToUse]->GetRotationFloat3().y,
			gfx.cube->GetRotationFloat3().z
		);
	}

	// Set position of spot light model
	XMVECTOR spotLightPosition = gfx.cameras["Default"]->GetPositionVector();
	spotLightPosition += gfx.cameras["Default"]->GetForwardVector() / 4;
	spotLightPosition += gfx.cameras["Default"]->GetRightVector() / 2;
	gfx.spotLight.SetPosition( spotLightPosition );
	gfx.spotLight.SetRotation(
		gfx.cameras["Default"]->GetRotationFloat3().x + XM_PI,
		gfx.cameras["Default"]->GetRotationFloat3().y,
		gfx.cameras["Default"]->GetRotationFloat3().z
	);

	gfx.Update( dt );
}

void Application::Render()
{
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}