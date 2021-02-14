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

	mousePick.Initialize( gfx.GetCamera( gfx.cameraToUse )->GetViewMatrix(),
		gfx.GetCamera( gfx.cameraToUse )->GetProjectionMatrix(), width, height );

	return true;
}

bool Application::ProcessMessages() noexcept
{
	return renderWindow.ProcessMessages();
}

void Application::Update()
{
	float dt = static_cast<float>( timer.GetMilliSecondsElapsed() );
	timer.Restart();

    while ( !keyboard.CharBufferIsEmpty() )
	{
		unsigned char ch = keyboard.ReadChar();
		UNREFERENCED_PARAMETER( ch );
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
					gfx.GetCamera( gfx.cameraToUse )->AdjustRotation(
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
		mousePick.UpdateMatrices( gfx.GetCamera( gfx.cameraToUse )->GetViewMatrix(),
			gfx.GetCamera( gfx.cameraToUse )->GetProjectionMatrix() );

		if ( mousePick.TestIntersection( gfx.GetWidth() / 2, gfx.GetHeight() / 2, gfx.GetCube() ) )
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
				case 0: gfx.selectedBox = "Basic"; break;
				case 1: gfx.selectedBox = "Bounce"; break;
				case 2: gfx.selectedBox = "Arrow"; break;
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
		gfx.GetCamera( "Static" )->SetLookAtPos( gfx.GetCamera( "Default" )->GetPositionFloat3() );
	}
	else
	{
		// update mode to ignore y-movement when not in debug mode
		bool playMode = true;
		if ( gfx.cameraToUse == "Debug" )
		{
			playMode = false;
			if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( gfx.GetCamera( "Debug" ), dt );
			if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( gfx.GetCamera( "Debug" ), dt );
		}
		gfx.GetCamera( gfx.cameraToUse )->SetCameraSpeed( 0.01f );
		if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( gfx.GetCamera( gfx.cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( gfx.GetCamera( gfx.cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( gfx.GetCamera( gfx.cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( gfx.GetCamera( gfx.cameraToUse ), playMode, dt );
	}

	// set multi-tool type
	if ( keyboard.KeyIsPressed( '1' ) ) gfx.toolType = gfx.CONVERT;
	if ( keyboard.KeyIsPressed( '2' ) ) gfx.toolType = gfx.RESIZE;

	// pick-up cube - set position relative to camera
	if ( keyboard.KeyIsPressed( 'E' ) && gfx.cameraToUse != "Static" && gfx.cubeInRange && gfx.cubeHover )
	{
		gfx.holdingCube = true;
		XMVECTOR cubePosition = gfx.GetCamera( gfx.cameraToUse )->GetPositionVector();
		cubePosition += gfx.GetCamera( gfx.cameraToUse )->GetForwardVector() * 2;
		gfx.GetCube().SetPosition( cubePosition );
		gfx.GetCube().SetRotation(
			gfx.GetCube().GetRotationFloat3().x,
			gfx.GetCamera( gfx.cameraToUse )->GetRotationFloat3().y,
			gfx.GetCube().GetRotationFloat3().z
		);
	}
	else
	{
		gfx.holdingCube = false;
	}

	gfx.Update( dt );
}

void Application::Render()
{
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}