#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( Graphics* gfx, RenderWindow& window,CameraController* camera, int width, int height )
{
	DisableCursor();
	this->graphics = gfx;
	this->renderWindow = window;
	this->cameras = camera;
	mousePick.Initialize( width, height );
}

void Input::Update( const float dt )
{
	UpdateKeyboard( dt );
	UpdateMouse( dt );
}

void Input::UpdateKeyboard( const float dt )
{
	// set camera to use
	if ( keyboard.KeyIsPressed( VK_F1 ) ) cameras->SetCurrentCamera(JSON::CameraType::Default);
	if ( keyboard.KeyIsPressed( VK_F2 ) ) cameras->SetCurrentCamera(JSON::CameraType::Static);
	if ( keyboard.KeyIsPressed( VK_F3 ) ) cameras->SetCurrentCamera(JSON::CameraType::Debug);

	// set cursor enabled/disabled
	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
	{
		if ( keyboard.KeyIsPressed( VK_HOME ) && !cursorEnabled ) EnableCursor();
		else if ( keyboard.KeyIsPressed( VK_END ) && cursorEnabled ) DisableCursor();
	}
	else
	{
		DisableCursor();
	}

	// camera movement
	if (cameras->GetCurrentCamera() == JSON::CameraType::Static )
	{
		cameras->GetCamera( JSON::CameraType::Static )->SetLookAtPos(
			cameras->GetCamera( JSON::CameraType::Default )->GetPositionFloat3() );
	}
	else
	{
		// update mode to ignore y-movement when not in debug mode. Will be changed in the future likely when player can move around the enviroment with physics / collisions.
		//Will also need to be changed to the player object when player becomes its own class. Unknown how that will work currently
		bool playMode = true;
		if (cameras->GetCurrentCamera() == JSON::CameraType::Debug )
		{
			playMode = false;
			if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( cameras->GetCamera( JSON::CameraType::Debug ), dt );
			if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown(cameras->GetCamera( JSON::CameraType::Debug ), dt );
		}
		cameras->GetCamera(cameras->GetCurrentCamera())->SetCameraSpeed( 0.01f );
		if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward(cameras->GetCamera(cameras->GetCurrentCamera()), playMode, dt );
		if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft(cameras->GetCamera(cameras->GetCurrentCamera()), playMode, dt );
		if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward(cameras->GetCamera(cameras->GetCurrentCamera()), playMode, dt );
		if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight(cameras->GetCamera(cameras->GetCurrentCamera()), playMode, dt );
	}

	// set multi-tool type
	if ( keyboard.KeyIsPressed( '1' ) ) graphics->GetCube().GetEditableProperties()->SetType( ToolType::CONVERT );
	if ( keyboard.KeyIsPressed( '2' ) ) graphics->GetCube().GetEditableProperties()->SetType( ToolType::RESIZE );

	// pick-up cube - set position relative to camera.
	if ( keyboard.KeyIsPressed( 'E' ) &&
		cameras->GetCurrentCamera() != JSON::CameraType::Static &&
		graphics->cubeInRange &&
		( graphics->cubeHover || graphics->GetCube().GetIsHolding() ) )
	{
		graphics->GetCube().SetIsHolding( true );
		XMVECTOR cubePosition = cameras->GetCamera(cameras->GetCurrentCamera())->GetPositionVector();
		cubePosition += cameras->GetCamera(cameras->GetCurrentCamera())->GetForwardVector() * 2;
		graphics->GetCube().SetPosition( cubePosition );
		graphics->GetCube().SetRotation(
			graphics->GetCube().GetRotationFloat3().x,
			cameras->GetCamera(cameras->GetCurrentCamera())->GetRotationFloat3().y,
			graphics->GetCube().GetRotationFloat3().z
		);
	}
	else
	{
		graphics->GetCube().SetIsHolding( false );
	}
}

void Input::UpdateMouse( const float dt )
{
	// read mouse events
	while ( !mouse.EventBufferIsEmpty() )
	{
		Mouse::MouseEvent me = mouse.ReadEvent();
		if (cameras->GetCurrentCamera() != JSON::CameraType::Static )
		{
			if ( mouse.IsRightDown() || !cursorEnabled )
			{
				if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
				{
					cameras->GetCamera(cameras->GetCurrentCamera())->AdjustRotation(
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
		mousePick.UpdateMatrices( cameras->GetCamera(cameras->GetCurrentCamera()) );

		if ( mousePick.TestIntersection( graphics->GetWidth() / 2, graphics->GetHeight() / 2, graphics->GetCube() ) )
			graphics->cubeHover = true;
		else
			graphics->cubeHover = false;

		// manage multi-tool options
		if ( graphics->GetCube().GetEditableProperties()->GetType() == ToolType::CONVERT )
		{
			// change selected texture to use on box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && graphics->boxToUse < 3 )
				graphics->boxToUse++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && graphics->boxToUse > 0 )
				graphics->boxToUse--;

			// update box texture on click while hovering
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->cubeHover )
			{
				switch ( graphics->boxToUse ) // might want to refactor this and how it worked (using enums instead of strings as well)
				{
				case 0: graphics->selectedBox = "Basic"; break;
				case 1: graphics->selectedBox = "Bounce"; break;
				case 2: graphics->selectedBox = "Arrow"; break;
				case 3: graphics->selectedBox = "TNT"; break;
				}
			}
		}
		else if ( graphics->GetCube().GetEditableProperties()->GetType() == ToolType::RESIZE )
		{
			// change size amount to change box to
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && graphics->sizeAmount < 2 )
				graphics->sizeAmount++;
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && graphics->sizeAmount > 0 )
				graphics->sizeAmount--;

			// set the box scale to use based on the option previously selected
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->cubeHover )
			{
				switch ( graphics->sizeAmount )
				{
				case 0: graphics->sizeToUse = 0.25f; break;
				case 1: graphics->sizeToUse = 1.0f; break;
				case 2: graphics->sizeToUse = 2.0f; break;
				}
			}
		}
	}
}