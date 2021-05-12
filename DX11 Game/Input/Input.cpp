#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( RenderWindow& window, LevelStateMachine* stateMachine,
	CameraController* camera, Sound* sound, std::vector<uint32_t> level_IDs )
{
	cameras = camera;
	soundSystem = sound;
	renderWindow = window;
	this->level_IDs = level_IDs;
	this->levelSystem = stateMachine;
	this->level = &*stateMachine->GetCurrentLevel();

	keyboard.DisableAutoRepeatKeys();
	mousePick.Initialize(
		static_cast<int>( renderWindow.GetWidth() ),
		static_cast<int>( renderWindow.GetHeight() )
	);
	
	AddToEvent();
}

void Input::Update( const float dt )
{
	UpdateKeyboard( dt );
	UpdateMouse( dt );
}

void Input::AddToEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::GamePauseEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::GameUnPauseEvent, this);
	
}

void Input::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::GamePauseEvent:
	{
			EnableCursor();
	}
	break;
	case EVENTID::GameUnPauseEvent:
	{
		UiMouseData.LPress = false;
		UiMouseData.MPress = false;
		UiMouseData.RPress = false;
		DisableCursor();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		DirectX::XMFLOAT2 _SizeOfScreen = *static_cast<DirectX::XMFLOAT2*>(event->GetData());
		mousePick.SetWidthHight(_SizeOfScreen.x, _SizeOfScreen.y);

		if (
			mouse.GetPosX() <= 0 &&
			mouse.GetPosX() >= (0 + _SizeOfScreen.x) &&
			mouse.GetPosY() <= 0 &&
			mouse.GetPosY() >= (0 + _SizeOfScreen.y)) {
			UiMouseData.LPress = false;
			UiMouseData.MPress = false;
			UiMouseData.RPress = false;
		}
		

	} 
	break;
	case EVENTID::UpdateSettingsEvent:
	{
		//controlls 
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>(event->GetData());
		for (auto& setting : a)
		{
			//only for player not debug key changes
			if (setting.Type == JSON::SettingType::ControllType) {
				//change controll

				//control map
				string key =std::get<string>(setting.Setting).c_str();

				//convert to input commands
				
				if (key == "SCROLL WHEEL") {
					MouseBindes[setting.Name+"_Up"] = Mouse::MouseEvent::EventType::WheelUp;
					MouseBindes[setting.Name+"_Down"] = Mouse::MouseEvent::EventType::WheelDown;
				}
				else if (key == "RMB")
				{
					MouseBindes[setting.Name] = Mouse::MouseEvent::EventType::RPress;
				}
				else if (key == "LMB")
				{
					MouseBindes[setting.Name] = Mouse::MouseEvent::EventType::LPress;
				}
				else if (key == "MMB")
				{
					MouseBindes[setting.Name] = Mouse::MouseEvent::EventType::MPress;
				}
				else
				{
					unsigned char* valChar= (unsigned char*)key.c_str();
					KeyBindes[setting.Name]= *valChar;
				}
				
			}
			//mouse inputs
				MouseBindes["Change_Gun_State_Up"] = Mouse::MouseEvent::EventType::WheelUp;
				MouseBindes["Change_Gun_State_Down"] = Mouse::MouseEvent::EventType::WheelDown;
				MouseBindes["Fire_Tool"] = Mouse::MouseEvent::EventType::LPress;

		}
		
	}
	break;
	}
}

void Input::UpdateKeyboard( const float dt )
{
#pragma region KeyPress_Once
	// handle input for single key presses
	while ( !keyboard.KeyBufferIsEmpty() )
	{
		unsigned char keycode = keyboard.ReadKey().GetKeyCode();

		// LEVEL SELECTION
		{
			if ( keycode == VK_NUMPAD1 ) levelSystem->SwitchTo( level_IDs[0] );
			if ( keycode == VK_NUMPAD2 ) levelSystem->SwitchTo( level_IDs[1] );
		}

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
				if ( keycode == KeyBindes["Gun_State_One"]) level->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Convert );
				if ( keycode == KeyBindes["Gun_State_Two"]) level->GetCube()[i]->GetEditableProperties()->SetToolType( ToolType::Resize );
			}
		}


		//UI
		{
			UIChar = keycode;
			EventSystem::Instance()->AddEvent(EVENTID::UIKeyInput, &UIChar);


			if (keycode == KeyBindes["Pause"]) {
				//puase game
				EventSystem::Instance()->AddEvent(EVENTID::GamePauseEvent);
			
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
			if ( keyboard.KeyIsPressed(KeyBindes["Jump"]) || jumping )
				CameraMovement::Jump( cameras->GetCamera( JSON::CameraType::Default ), jumping, dt );
		}
	
		// normalize diagonal movement speed
		if ( keyboard.KeyIsPressed(KeyBindes["Forward"]) && ( keyboard.KeyIsPressed(KeyBindes["Left"]) || keyboard.KeyIsPressed(KeyBindes["Back"]) ) )
			cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.005f );
		if ( keyboard.KeyIsPressed(KeyBindes["Back"]) && ( keyboard.KeyIsPressed(KeyBindes["Left"]) || keyboard.KeyIsPressed(KeyBindes["Back"]) ) )
			cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.005f );

		// update camera movement
		if ( keyboard.KeyIsPressed(KeyBindes["Forward"]) ) CameraMovement::MoveForward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBindes["Left"]) ) CameraMovement::MoveLeft( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBindes["Back"]) ) CameraMovement::MoveBackward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBindes["Right"]) ) CameraMovement::MoveRight( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );

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
			if ( keyboard.KeyIsPressed(KeyBindes["Action"]) && !alreadyHeld &&
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
				// testing sound, feel free to move or remove
				if ( me.GetType() == Mouse::MouseEvent::EventType::LPress )
				{
					soundSystem->PlaySoundEffects( soundSystem->SOUND_TOOLUSE, XMFLOAT3(0.0f, 0.0f, 0.0f) );
				}

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
					if ( me.GetType() == MouseBindes["Change_Gun_State_Up"] &&
						level->GetCube()[i]->GetEditableProperties()->GetMaterialID() < 3 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetMaterialID(
							level->GetCube()[i]->GetEditableProperties()->GetMaterialID() + 1
						);
					}
					else if ( me.GetType() == MouseBindes["Change_Gun_State_Down"] &&
						level->GetCube()[i]->GetEditableProperties()->GetMaterialID() > 0 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetMaterialID(
							level->GetCube()[i]->GetEditableProperties()->GetMaterialID() - 1
						);
					}

					// update box texture on click while hovering
					if ( me.GetType() == MouseBindes["Fire_Tool"] && level->GetCube()[i]->GetIsHovering() )
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
					if ( me.GetType() == MouseBindes["Change_Gun_State_Up"] &&
						level->GetCube()[i]->GetEditableProperties()->GetSizeID() < 2 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetSizeID(
							level->GetCube()[i]->GetEditableProperties()->GetSizeID() + 1
						);
					}
					else if ( me.GetType() == MouseBindes["Change_Gun_State_Down"] &&
						level->GetCube()[i]->GetEditableProperties()->GetSizeID() > 0 )
					{
						level->GetCube()[i]->GetEditableProperties()->SetSizeID(
							level->GetCube()[i]->GetEditableProperties()->GetSizeID() - 1
						);
					}

					// set the box scale to use based on the option previously selected
					if ( me.GetType() == MouseBindes["Fire_Tool"] && level->GetCube()[i]->GetIsHovering() )
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
			//UI mouse input
			{
				UiMouseData.Pos = { static_cast<float>(me.GetPosX()),static_cast<float>(me.GetPosY()) };
				if (mouse.IsRightDown() && cursorEnabled) {
					UiMouseData.RPress = true;
				}
				else
				{
					UiMouseData.RPress = false;
				}
				if (mouse.IsLeftDown() && cursorEnabled) {
					UiMouseData.LPress = true;
				}
				else
				{
					UiMouseData.LPress = false;
				}
				if (mouse.IsMiddleDown() && cursorEnabled) {
					UiMouseData.MPress = true;
				}
				else
				{
					UiMouseData.MPress = false;
				}

				EventSystem::Instance()->AddEvent(EVENTID::UIMouseInput, &UiMouseData);

			}
		}
	}
}