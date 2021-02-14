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
		if ( gfx.cameraToUse != "Static" )
		{
			if ( mouse.IsRightDown() || !cursorEnabled )
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
		}
		
		// mouse picking
		mousePick.UpdateMatrices( gfx.cameras[gfx.cameraToUse]->GetViewMatrix(),
			gfx.cameras[gfx.cameraToUse]->GetProjectionMatrix() );
		if ( mousePick.TestIntersection( gfx.GetWidth() / 2, gfx.GetHeight() / 2, *gfx.cube.get() ) )
			gfx.cubeHover = true;
		else
			gfx.cubeHover = false;
		
		// manage multi-tool options
		if ( gfx.toolType == gfx.CONVERT )
		{
			// change selected texture to use on box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && gfx.boxToUse < 3 )
				gfx.boxToUse++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && gfx.boxToUse > 0 )
				gfx.boxToUse--;

			// update box texture on click while hovering
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
			// change size amount to change box to
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && gfx.sizeAmount < 2 )
				gfx.sizeAmount++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && gfx.sizeAmount > 0 )
				gfx.sizeAmount--;

			// set the box scale to use based on the option previously selected
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

	// set camera to use
	if ( keyboard.KeyIsPressed( VK_F1 ) ) gfx.cameraToUse = "Default";
	if ( keyboard.KeyIsPressed( VK_F2 ) ) gfx.cameraToUse = "Static";
	if ( keyboard.KeyIsPressed( VK_F3 ) ) gfx.cameraToUse = "Debug";

	// set cursor enabled/disabled
	if ( gfx.cameraToUse == "Debug" )
	{
		if ( keyboard.KeyIsPressed( VK_HOME ) && !cursorEnabled )
		{
			EnableCursor();
			cursorEnabled = true;
		}
		else if ( keyboard.KeyIsPressed( VK_END ) && cursorEnabled )
		{
			DisableCursor();
			cursorEnabled = false;
		}
	}
	else
	{
		DisableCursor();
		cursorEnabled = false;
	}

	// camera movement
	if ( gfx.cameraToUse == "Static" )
	{
		gfx.cameras["Static"]->SetLookAtPos( gfx.cameras["Default"]->GetPositionFloat3() );
	}
	else
	{
		gfx.cameras[gfx.cameraToUse]->SetCameraSpeed( 0.002f );
		if ( keyboard.KeyIsPressed( VK_SHIFT ) ) gfx.cameras[gfx.cameraToUse]->SetCameraSpeed( 0.01f );
		if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( gfx.cameras[gfx.cameraToUse], dt );
		if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( gfx.cameras[gfx.cameraToUse], dt );
		if ( gfx.cameraToUse == "Debug" )
		{
			if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( gfx.cameras["Debug"], dt );
			if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( gfx.cameras["Debug"], dt );
		}
	}

	// camera world collisions
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

	// set multi-tool type
	if ( keyboard.KeyIsPressed( '1' ) ) gfx.toolType = gfx.CONVERT;
	if ( keyboard.KeyIsPressed( '2' ) ) gfx.toolType = gfx.RESIZE;

	// pick-up cube - set position relative to camera
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

	gfx.Update( dt );
}

void Application::Render()
{
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}