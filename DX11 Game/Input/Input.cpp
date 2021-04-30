#include "stdafx.h"
#include "Input.h"
#include "Sound.h"
#include "CameraMovement.h"

void Input::Initialize( Level* gfx, RenderWindow& window, CameraController* camera, int width, int height )
{
	this->level = gfx;
	this->cameras = camera;
	this->renderWindow = window;
	keyboard.DisableAutoRepeatKeys();
	mousePick.Initialize( width, height );
}

void Input::Update( const float dt, Sound sound )
{
	UpdateKeyboard( dt );
	UpdateMouse( dt );

	if ( keyboard.KeyIsPressed( 'B' ) )
		sound.PlayWavFile( sound.COLLISION_SOUND, 1.0f, level->GetCube()[0]->GetPositionFloat3() );
}

void Input::UpdateKeyboard( const float dt )
{
#pragma region KeyPress_Once
	// handle input for single key presses
	while ( !keyboard.KeyBufferIsEmpty() )
	{
		unsigned char keycode = keyboard.ReadKey().GetKeyCode();

		// CAMERA INPUT
		{
			// set camera to use
			if ( keycode == VK_F1 ) level->GetCameraController()->SetIsUsingMain( true );
			if ( keycode == VK_F2 ) level->GetCameraController()->SetIsUsingMain( false );

			// set options for debug camera
			if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
			{
				// set cursor enabled/disabled
				if ( keycode == VK_HOME && !cursorEnabled ) EnableCursor();
				else if ( keycode == VK_END && cursorEnabled ) DisableCursor();
			}
		}

		// MULTI-TOOL INPUT
		{
			for ( uint32_t i = 0; i < NUM_CUBES; i++ )
			{
				if ( keycode == '1' ) level->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Convert );
				if ( keycode == '2' ) level->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Resize );
			}
		}
	}
#pragma endregion

#pragma region KeyPress_Repeat
	// CAMERA INPUT
	{
		// TODO: THIS SHOULD GO INTO AN UPDATE FUNCTION IN THE CAMERA CLASS
		// set which camera for the static camera to look at
		cameras->GetCamera( JSON::CameraType::Static )->SetLookAtPos(
			cameras->GetCamera( level->GetCameraController()->GetCurrentCamera() )->GetPositionFloat3() );

		// update mode to ignore y-movement when not in debug mode. Will be changed in the future likely when player can move around the environment with physics/collisions.
		// will also need to be changed to the player object when player becomes its own class. Unknown how that will work currently
		bool playMode = true;
		if ( cameras->GetCurrentCamera() == JSON::CameraType::Debug )
		{
			playMode = false;
			if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( cameras->GetCamera( JSON::CameraType::Debug ), dt );
			if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( cameras->GetCamera( JSON::CameraType::Debug ), dt );
		}
		else
		{
			static bool jumping = false;
			if ( keyboard.KeyIsPressed( VK_SPACE ) || jumping )
				CameraMovement::Jump( cameras->GetCamera( JSON::CameraType::Default ), jumping, dt );
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
	}

	// MULTI-TOOL INPUT
	{
		// set multi-tool type
		for ( uint32_t i = 0; i < NUM_CUBES; i++ )
		{
			// ensure another cube is not already being held
			float alreadyHeld = false;
			for ( uint32_t j = 0; j < NUM_CUBES; j++ )
				if ( i != j && level->GetCube()[j]->GetIsHolding() == true )
					alreadyHeld = true;

			// pick-up cube - set position relative to camera.
			if ( keyboard.KeyIsPressed( 'E' ) && !alreadyHeld &&
				 level->GetCube()[i]->GetIsInRange() &&
				( level->GetCube()[i]->GetIsHovering() ||
				  level->GetCube()[i]->GetIsHolding() ) )
			{
				level->GetCube()[i]->SetIsHolding( true );
				XMVECTOR cubePosition = cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionVector();
				cubePosition += cameras->GetCamera( cameras->GetCurrentCamera() )->GetForwardVector() * 2;
				level->GetCube()[i]->SetPosition( cubePosition );
				level->GetCube()[i]->SetRotation(
					level->GetCube()[i]->GetRotationFloat3().x,
					cameras->GetCamera( cameras->GetCurrentCamera() )->GetRotationFloat3().y,
					level->GetCube()[i]->GetRotationFloat3().z
				);
			}
			else
			{
				level->GetCube()[i]->SetIsHolding( false );
			}
		}
	}

	// CUBE INPUT
	{
		// update cube movement
		if ( keyboard.KeyIsPressed( VK_RIGHT ) )
			level->GetCube()[0]->GetPhysicsModel()->AddForce( { 0.1f, 0.0f, 0.0f } );
		if ( keyboard.KeyIsPressed( VK_LEFT ) )
			level->GetCube()[0]->GetPhysicsModel()->AddForce( { -0.1f, 0.0f, 0.0f } );
	}
#pragma endregion
}

void Input::UpdateMouse( const float dt )
{
	// read mouse events
	while ( !mouse.EventBufferIsEmpty() )
	{
		Mouse::MouseEvent me = mouse.ReadEvent();

		// CAMERA INPUT
		{
			// camera orientation
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
		}

		// MULTI-TOOL INPUT
		{
			// mouse picking
			mousePick.UpdateMatrices( cameras->GetCamera( cameras->GetCurrentCamera() ) );

			for ( uint32_t i = 0; i < NUM_CUBES; i++ )
			{
				// test intersection between crosshair and cube
				if ( mousePick.TestIntersection( level->GetGraphics()->GetWidth() / 2, level->GetGraphics()->GetHeight() / 2, *level->GetCube()[i] ) )
					level->GetCube()[i]->SetIsHovering( true );
				else
					level->GetCube()[i]->SetIsHovering( false );

#pragma region Tool_Convert
				// manage multi-tool options
				if ( level->GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Convert )
				{
					// change current id of texture to be used on box
					if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
						level->GetCube()[i]->GetEditableProperties()->GetMaterialID() < 3 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetMaterialID(
							level->GetCube()[i]->GetEditableProperties()->GetMaterialID() + 1
						);
					}
					else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
						level->GetCube()[i]->GetEditableProperties()->GetMaterialID() > 0 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetMaterialID(
							level->GetCube()[i]->GetEditableProperties()->GetMaterialID() - 1
						);
					}

					// update box texture on click while hovering
					if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && level->GetCube()[i]->GetIsHovering() )
					{
						level->GetCube()[i]->GetEditableProperties()->SetBoxType(
							static_cast<BoxType>( level->GetCube()[i]->GetEditableProperties()->GetMaterialID() )
						);
					}
				}
#pragma endregion

#pragma region Tool_Resize
				else if ( level->GetCube()[i]->GetEditableProperties()->GetToolType() == ToolType::Resize )
				{
					// set size multiplier to be applied to the box
					if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
						level->GetCube()[i]->GetEditableProperties()->GetSizeID() < 2 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetSizeID(
							level->GetCube()[i]->GetEditableProperties()->GetSizeID() + 1
						);
					}
					else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
						level->GetCube()[i]->GetEditableProperties()->GetSizeID() > 0 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetSizeID(
							level->GetCube()[i]->GetEditableProperties()->GetSizeID() - 1
						);
					}

					// set the box scale to use based on the option previously selected
					if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && level->GetCube()[i]->GetIsHovering() )
					{
						switch ( level->GetCube()[i]->GetEditableProperties()->GetSizeID() )
						{
						case 0:
							level->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 0.5f );
							level->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Small );
							break;
						case 1:
							level->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 1.0f );
							level->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Normal );
							break;
						case 2:
							level->GetCube()[i]->GetEditableProperties()->SetSizeMultiplier( 2.0f );
							level->GetCube()[i]->GetEditableProperties()->SetBoxSize( BoxSize::Large );
							break;
						}
					}
				}
#pragma endregion
			}
		}
	}
}