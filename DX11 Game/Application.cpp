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

	mousePick.Initialize( gfx.camera->GetViewMatrix(), gfx.camera->GetProjectionMatrix(), width, height );

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
				gfx.camera->AdjustRotation(
					XMFLOAT3(
						static_cast<float>( me.GetPosY() ) * 0.005f,
						static_cast<float>( me.GetPosX() ) * 0.005f,
						0.0f
					)
				);
			}
		}
		
		// Mouse Picking
		mousePick.UpdateMatrices( gfx.camera->GetViewMatrix(), gfx.camera->GetProjectionMatrix() );
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

	// Camera Movement
	gfx.camera->SetCameraSpeed( 0.002f );
	if ( keyboard.KeyIsPressed( VK_SHIFT ) ) gfx.camera->SetCameraSpeed( 0.01f );
	if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( gfx.camera, dt );

	// Camera World Collisions
	if ( !gfx.wallCollision )
	{
		float dx = gfx.hubRoom.GetPositionFloat3().x - gfx.camera->GetPositionFloat3().x;
		float dz = gfx.hubRoom.GetPositionFloat3().z - gfx.camera->GetPositionFloat3().z;
		float length = std::sqrtf( dx * dx + dz * dz );
		dx /= length;
		dz /= length;
		dx *= gfx.camera->GetCameraSpeed() * 10.0f;
		dz *= gfx.camera->GetCameraSpeed() * 10.0f;
		gfx.camera->AdjustPosition( dx, 0.0f, dz );
	}

	// Prevent Camera Y-Axis Movement
	gfx.camera->SetPosition( gfx.camera->GetPositionFloat3().x, 9.0f, gfx.camera->GetPositionFloat3().z );

	// Multi-Tool Type
	if ( keyboard.KeyIsPressed( '1' ) ) gfx.toolType = gfx.CONVERT;
	if ( keyboard.KeyIsPressed( '2' ) ) gfx.toolType = gfx.RESIZE;

	// Pick-Up Cube - Set position relative to camera
	if ( keyboard.KeyIsPressed( 'E' ) )
	{
		XMVECTOR cubePosition = gfx.camera->GetPositionVector();
		cubePosition += gfx.camera->GetForwardVector() * 2;
		gfx.cube->SetPosition( cubePosition );
		gfx.cube->SetRotation(
			gfx.cube->GetRotationFloat3().x,
			gfx.camera->GetRotationFloat3().y,
			gfx.cube->GetRotationFloat3().z
		);
	}

	// Set position of spot light model
	XMVECTOR spotLightPosition = gfx.camera->GetPositionVector();
	spotLightPosition += gfx.camera->GetForwardVector() / 4;
	spotLightPosition += gfx.camera->GetRightVector() / 2;
	gfx.spotLight.SetPosition( spotLightPosition );
	gfx.spotLight.SetRotation(
		gfx.camera->GetRotationFloat3().x + XM_PI,
		gfx.camera->GetRotationFloat3().y,
		gfx.camera->GetRotationFloat3().z
	);

	gfx.Update( dt );
}

void Application::Render()
{
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}