#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( Graphics* gfx, RenderWindow& window, CameraController* camera,ObjectManager* objects, int width, int height )
{
	DisableCursor();
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
	else
	{
		DisableCursor();
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

	//Also super messy because dont want to confuse the system using multiple cubes for now
	Cube* cubez = objects->GetCubes()[0];

	// set multi-tool type
	if ( keyboard.KeyIsPressed( '1' ) ) cubez->GetEditableProperties()->SetToolType( ToolType::Convert );
	if ( keyboard.KeyIsPressed( '2' ) ) cubez->GetEditableProperties()->SetToolType( ToolType::Resize );

	for (auto cube : objects->GetCubes())
	{
		// pick-up cube - set position relative to camera.
		if (keyboard.KeyIsPressed('E') && cube->GetIsInRange() &&
			(cube->GetIsHovering() || cube->GetIsHolding()))
		{
			cube->SetIsHolding(true);
			XMVECTOR cubePosition = cameras->GetCamera(cameras->GetCurrentCamera())->GetPositionVector();
			cubePosition += cameras->GetCamera(cameras->GetCurrentCamera())->GetForwardVector() * 2;
			cube->SetPosition(cubePosition);
			cube->SetRotation(
				cube->GetRotationFloat3().x,
				cameras->GetCamera(cameras->GetCurrentCamera())->GetRotationFloat3().y,
				cube->GetRotationFloat3().z
			);
		}
		else
		{
			cube->SetIsHolding(false);
		}
	}
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

		//Super messy because dont want to confuse the system using multiple cubes for now
		Cube* cubez = objects->GetCubes()[0];


		for (auto cube : objects->GetCubes())
		{
			if (mousePick.TestIntersection(graphics->GetWidth() / 2, graphics->GetHeight() / 2, *cube))
				cube->SetIsHovering(true);
			else
				cube->SetIsHovering(false);
		}



		// manage multi-tool options
		if ( cubez->GetEditableProperties()->GetToolType() == ToolType::Convert )
		{
			// change current id of texture to be used on box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
				cubez->GetEditableProperties()->GetMaterialID() < 3 )
			{
				cubez->GetEditableProperties()->SetMaterialID(
					cubez->GetEditableProperties()->GetMaterialID() + 1
				);
			}
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
				cubez->GetEditableProperties()->GetMaterialID() > 0 )
			{
				cubez->GetEditableProperties()->SetMaterialID(
					cubez->GetEditableProperties()->GetMaterialID() - 1
				);
			}

			// update box texture on click while hovering
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && cubez->GetIsHovering() )
			{
				cubez->GetEditableProperties()->SetBoxType(
					static_cast<BoxType>(cubez->GetEditableProperties()->GetMaterialID() )
				);
			}
		}
		else if (cubez->GetEditableProperties()->GetToolType() == ToolType::Resize )
		{
			// set size multiplier to be applied to the box
			if ( me.GetType() == Mouse::MouseEvent::EventType::WheelUp &&
				cubez->GetEditableProperties()->GetSizeID() < 2 )
			{
				cubez->GetEditableProperties()->SetSizeID(
					cubez->GetEditableProperties()->GetSizeID() + 1
				);
			}
			else if ( me.GetType() == Mouse::MouseEvent::EventType::WheelDown &&
				cubez->GetEditableProperties()->GetSizeID() > 0 )
			{
				cubez->GetEditableProperties()->SetSizeID(
					cubez->GetEditableProperties()->GetSizeID() - 1
				);
			}

			// set the box scale to use based on the option previously selected
			if ( me.GetType() == Mouse::MouseEvent::EventType::LPress && cubez->GetIsHovering() )
			{
				switch (cubez->GetEditableProperties()->GetSizeID() )
				{
				case 0:
					cubez->GetEditableProperties()->SetSizeMultiplier( 0.5f );
					cubez->GetEditableProperties()->SetBoxSize( BoxSize::Small );
					break;
				case 1:
					cubez->GetEditableProperties()->SetSizeMultiplier( 1.0f );
					cubez->GetEditableProperties()->SetBoxSize( BoxSize::Normal );
					break;
				case 2:
					cubez->GetEditableProperties()->SetSizeMultiplier( 2.0f );
					cubez->GetEditableProperties()->SetBoxSize( BoxSize::Large );
					break;
				}
			}
		}
	}
}