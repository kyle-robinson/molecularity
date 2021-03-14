#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( Graphics* gfx, RenderWindow& window, CameraController* camera, int width, int height )
{
	this->graphics = gfx;
	this->cameras = camera;
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
	if ( keyboard.KeyIsPressed( VK_F1 ) ) graphics->GetCameraController()->SetIsUsingMain( true );
	if ( keyboard.KeyIsPressed( VK_F2 ) ) graphics->GetCameraController()->SetIsUsingMain( false );

	// set cursor enabled/disabled
	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
	{
		if ( keyboard.KeyIsPressed( VK_HOME ) && !cursorEnabled ) EnableCursor();
		else if ( keyboard.KeyIsPressed( VK_END ) && cursorEnabled ) DisableCursor();
	}

	// set which camera for the static camera to look at
	cameras->GetCamera( JSON::CameraType::Static )->SetLookAtPos(
		cameras->GetCamera( graphics->GetCameraController()->GetCurrentCamera() )->GetPositionFloat3() );

	// update mode to ignore y-movement when not in debug mode. Will be changed in the future likely when player can move around the environment with physics/collisions.
	// will also need to be changed to the player object when player becomes its own class. Unknown how that will work currently
	bool playMode = true;
	if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
	{
		playMode = false;
		if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( cameras->GetCamera( JSON::CameraType::Debug ), dt );
		if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( cameras->GetCamera( JSON::CameraType::Debug ), dt );
	}
	
	// normalize diagonal movement speed
	if ( keyboard.KeyIsPressed( 'W' ) && ( keyboard.KeyIsPressed( 'A' ) || keyboard.KeyIsPressed( 'D' ) ) )
		cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.005f );
	if ( keyboard.KeyIsPressed( 'S' ) && ( keyboard.KeyIsPressed( 'A' ) || keyboard.KeyIsPressed( 'D' ) ) )
		cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.005f );

	// update camera movement
	if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
	if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
	if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
	if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );

	// set camera speed
	cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.01f );

	// set multi-tool type
	for ( uint32_t i = 0; i < NUM_CUBES; i++ )
	{
		if ( keyboard.KeyIsPressed( '1' ) ) graphics->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Convert );
		if ( keyboard.KeyIsPressed( '2' ) ) graphics->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Resize );

		// pick-up cube - set position relative to camera.
		if ( keyboard.KeyIsPressed( 'E' ) && graphics->GetCube()[i]->GetIsInRange() &&
			( graphics->GetCube()[i]->GetIsHovering() || graphics->GetCube()[i]->GetIsHolding() ) )
		{
			graphics->GetCube()[i]->SetIsHolding( true );
			XMVECTOR cubePosition = cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionVector();
			cubePosition += cameras->GetCamera( cameras->GetCurrentCamera() )->GetForwardVector() * 2;
			graphics->GetCube()[i]->SetPosition( cubePosition );
			graphics->GetCube()[i]->SetRotation(
				graphics->GetCube()[i]->GetRotationFloat3().x,
				cameras->GetCamera( cameras->GetCurrentCamera() )->GetRotationFloat3().y,
				graphics->GetCube()[i]->GetRotationFloat3().z
			);
		}
		else
		{
			graphics->GetCube()[i]->SetIsHolding( false );
		}
	}

	if ( keyboard.KeyIsPressed( VK_RIGHT ) )
		graphics->GetCube()[0]->GetPhysicsModel()->AddForce( { 0.1f, 0.0f, 0.0f } );
	if ( keyboard.KeyIsPressed( VK_LEFT ) )
		graphics->GetCube()[0]->GetPhysicsModel()->AddForce( { -0.1f, 0.0f, 0.0f } );
}

void Input::UpdateMouse( const float dt )
{
	// read mouse events
	while ( !mouse.EventBufferIsEmpty() )
	{
		Mouse::MouseEvent me = mouse.ReadEvent();
		if ( mouse.IsRightDown() || !cursorEnabled )
		{
			// update raw camera movement
			if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
			{
				cameras->GetCamera( cameras->GetCurrentCamera() )->AdjustRotation(
					XMFLOAT3(
						static_cast<float>( me.GetPosY() ) * 0.005f,
						static_cast<float>( me.GetPosX() ) * 0.005f,
						0.0f
					)
				);
			}
		}

		// mouse picking
		mousePick.UpdateMatrices( cameras->GetCamera( cameras->GetCurrentCamera() ) );

		for ( uint32_t i = 0; i < NUM_CUBES; i++ )
		{
			if ( mousePick.TestIntersection( graphics->GetWidth() / 2, graphics->GetHeight() / 2, *graphics->GetCube()[i] ) )
				graphics->GetCube()[i]->SetIsHovering( true );
			else
				graphics->GetCube()[i]->SetIsHovering( false );

			// manage multi-tool options
			if ( graphics->GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Convert )
			{
				// change current id of texture to be used on box
				if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
					graphics->GetCube()[i]->GetEditableProperties()->GetMaterialID() < 3 )
				{
					graphics->GetCube()[i]->GetEditableProperties()->SetMaterialID(
						graphics->GetCube()[i]->GetEditableProperties()->GetMaterialID() + 1
					);
				}
				else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
					graphics->GetCube()[i]->GetEditableProperties()->GetMaterialID() > 0 )
				{
					graphics->GetCube()[i]->GetEditableProperties()->SetMaterialID(
						graphics->GetCube()[i]->GetEditableProperties()->GetMaterialID() - 1
					);
				}

				// update box texture on click while hovering
				if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->GetCube()[i]->GetIsHovering() )
				{
					graphics->GetCube()[i]->GetEditableProperties()->SetBoxType(
						static_cast<BoxType>( graphics->GetCube()[i]->GetEditableProperties()->GetMaterialID() )
					);
				}
			}
			else if ( graphics->GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Resize )
			{
				// set size multiplier to be applied to the box
				if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
					graphics->GetCube()[i]->GetEditableProperties()->GetSizeID() < 2 )
				{
					graphics->GetCube()[i]->GetEditableProperties()->SetSizeID(
						graphics->GetCube()[i]->GetEditableProperties()->GetSizeID() + 1
					);
				}
				else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
					graphics->GetCube()[i]->GetEditableProperties()->GetSizeID() > 0 )
				{
					graphics->GetCube()[i]->GetEditableProperties()->SetSizeID(
						graphics->GetCube()[i]->GetEditableProperties()->GetSizeID() - 1
					);
				}

				// set the box scale to use based on the option previously selected
				if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && graphics->GetCube()[i]->GetIsHovering() )
				{
					switch ( graphics->GetCube()[i]->GetEditableProperties()->GetSizeID() )
					{
					case 0:
						graphics->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 0.5f );
						graphics->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Small );
						break;
					case 1:
						graphics->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 1.0f );
						graphics->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Normal );
						break;
					case 2:
						graphics->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 2.0f );
						graphics->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Large );
						break;
					}
				}
			}
		}
	}
}