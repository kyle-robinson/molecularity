#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( Graphics* gfx, RenderWindow& window, int width, int height )
{
	DisableCursor();
	this->graphics = gfx;
	this->renderWindow = window;
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
	if ( keyboard.KeyIsPressed( VK_F1 ) ) graphics->cameraToUse = JSON::CameraType::Default;
	if ( keyboard.KeyIsPressed( VK_F2 ) ) graphics->cameraToUse = JSON::CameraType::Static;
	if ( keyboard.KeyIsPressed( VK_F3 ) ) graphics->cameraToUse = JSON::CameraType::Debug;

	// set cursor enabled/disabled
	if ( graphics->cameraToUse == JSON::CameraType::Debug )
	{
		if ( keyboard.KeyIsPressed( VK_HOME ) && !cursorEnabled ) EnableCursor();
		else if ( keyboard.KeyIsPressed( VK_END ) && cursorEnabled ) DisableCursor();
	}
	else
	{
		DisableCursor();
	}

	// camera movement
	if ( graphics->cameraToUse == JSON::CameraType::Static )
	{
		graphics->GetCamera( JSON::CameraType::Static )->SetLookAtPos(
			graphics->GetCamera( JSON::CameraType::Default )->GetPositionFloat3() );
	}
	else
	{
		// update mode to ignore y-movement when not in debug mode
		bool playMode = true;
		if ( graphics->cameraToUse == JSON::CameraType::Debug )
		{
			playMode = false;
			if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( graphics->GetCamera( JSON::CameraType::Debug ), dt );
			if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( graphics->GetCamera( JSON::CameraType::Debug ), dt );
		}
		graphics->GetCamera( graphics->cameraToUse )->SetCameraSpeed( 0.01f );
		if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( graphics->GetCamera( graphics->cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( graphics->GetCamera( graphics->cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( graphics->GetCamera( graphics->cameraToUse ), playMode, dt );
		if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( graphics->GetCamera( graphics->cameraToUse ), playMode, dt );
	}

	// set multi-tool type
	if ( keyboard.KeyIsPressed( '1' ) ) graphics->GetCube().GetEditableProperties()->SetToolType( ToolType::Convert );
	if ( keyboard.KeyIsPressed( '2' ) ) graphics->GetCube().GetEditableProperties()->SetToolType( ToolType::Resize );

	// pick-up cube - set position relative to camera.
	if ( keyboard.KeyIsPressed( 'E' ) &&
		graphics->cameraToUse != JSON::CameraType::Static &&
		graphics->GetCube().GetIsInRange() &&
		( graphics->GetCube().GetIsHovering() || graphics->GetCube().GetIsHolding() ) )
	{
		graphics->GetCube().SetIsHolding( true );
		XMVECTOR cubePosition = graphics->GetCamera( graphics->cameraToUse )->GetPositionVector();
		cubePosition += graphics->GetCamera( graphics->cameraToUse )->GetForwardVector() * 2;
		graphics->GetCube().SetPosition( cubePosition );
		graphics->GetCube().SetRotation(
			graphics->GetCube().GetRotationFloat3().x,
			graphics->GetCamera( graphics->cameraToUse )->GetRotationFloat3().y,
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
		if ( graphics->cameraToUse != JSON::CameraType::Static )
		{
			if ( mouse.IsRightDown() || !cursorEnabled )
			{
				// update raw camera movement
				if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
				{
					graphics->GetCamera( graphics->cameraToUse )->AdjustRotation(
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
		mousePick.UpdateMatrices( graphics->GetCamera( graphics->cameraToUse ) );

		if ( mousePick.TestIntersection( graphics->GetWidth() / 2, graphics->GetHeight() / 2, graphics->GetCube() ) )
			graphics->GetCube().SetIsHovering( true );
		else
			graphics->GetCube().SetIsHovering( false );

		// manage multi-tool options
		if ( graphics->GetCube().GetEditableProperties()->GetToolType() == ToolType::Convert )
		{
			// change current id of texture to be used on box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
				graphics->GetCube().GetEditableProperties()->GetBoxMaterialID() < 3 )
			{
				graphics->GetCube().GetEditableProperties()->SetBoxMaterialID(
					graphics->GetCube().GetEditableProperties()->GetBoxMaterialID() + 1
				);
			}
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
				graphics->GetCube().GetEditableProperties()->GetBoxMaterialID() > 0 )
			{
				graphics->GetCube().GetEditableProperties()->SetBoxMaterialID(
					graphics->GetCube().GetEditableProperties()->GetBoxMaterialID() - 1
				);
			}

			// update box texture on click while hovering
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->GetCube().GetIsHovering() )
			{
				graphics->GetCube().GetEditableProperties()->SetBoxType(
					static_cast<BoxType>( graphics->GetCube().GetEditableProperties()->GetBoxMaterialID() )
				);
			}
		}
		else if ( graphics->GetCube().GetEditableProperties()->GetToolType() == ToolType::Resize )
		{
			static int sizeToUse = 1;

			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
				graphics->GetCube().GetEditableProperties()->GetBoxSize() < static_cast<BoxSize>( 2 ) )
			{
				graphics->GetCube().GetEditableProperties()->SetBoxSize( static_cast<BoxSize>( sizeToUse++ ) );
			}
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
				graphics->GetCube().GetEditableProperties()->GetBoxSize() > static_cast<BoxSize>( 0 ) )
			{
				graphics->GetCube().GetEditableProperties()->SetBoxSize( static_cast<BoxSize>( sizeToUse-- ) );
			}

			// change size amount to change box to
			//if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp && graphics->GetCube().GetSizeAmount() < 2 )
			//	graphics->GetCube().SetSizeAmount( graphics->GetCube().GetSizeAmount() + 1 );
			//else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown && graphics->GetCube().GetSizeAmount() > 0 )
			//	graphics->GetCube().SetSizeAmount( graphics->GetCube().GetSizeAmount() - 1 );

			// set the box scale to use based on the option previously selected
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->GetCube().GetIsHovering() )
			{
				switch ( sizeToUse )
				{
				//case 0: graphics->GetCube().SetSizeToUse( 0.25f ); break;
				//case 1: graphics->GetCube().SetSizeToUse( 1.0f ); break;
				//case 2: graphics->GetCube().SetSizeToUse( 2.0f ); break;
				case 0:
					graphics->GetCube().GetEditableProperties()->SetBoxSize( BoxSize::Small );
					graphics->GetCube().GetEditableProperties()->SetSizeMultiplier( 0.25f );
					break;
				case 1:
					graphics->GetCube().GetEditableProperties()->SetBoxSize( BoxSize::Normal );
					graphics->GetCube().GetEditableProperties()->SetSizeMultiplier( 1.0f );
					break;
				case 2:
					graphics->GetCube().GetEditableProperties()->SetBoxSize( BoxSize::Large );
					graphics->GetCube().GetEditableProperties()->SetSizeMultiplier( 2.0f );
					break;
				}
			}
		}
	}
}