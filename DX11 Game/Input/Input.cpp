#include "stdafx.h"
#include "Input.h"
#include "CameraMovement.h"

void Input::Initialize( RenderWindow& window, LevelStateMachine* stateMachine,
	CameraController* camera, std::vector<uint32_t> level_IDs )
{
	cameras = camera;
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
			isPaused = true;
	}
	break;
	case EVENTID::GameUnPauseEvent:
	{
		UiMouseData.LPress = false;
		UiMouseData.MPress = false;
		UiMouseData.RPress = false;
		DisableCursor();
		isPaused = false;
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
					MouseBinds[setting.Name+"_Up"] = Mouse::MouseEvent::EventType::WheelUp;
					MouseBinds[setting.Name+"_Down"] = Mouse::MouseEvent::EventType::WheelDown;
				}
				else if (key == "RMB")
				{
					MouseBinds[setting.Name] = Mouse::MouseEvent::EventType::RPress;
				}
				else if (key == "LMB")
				{
					MouseBinds[setting.Name] = Mouse::MouseEvent::EventType::LPress;
				}
				else if (key == "MMB")
				{
					MouseBinds[setting.Name] = Mouse::MouseEvent::EventType::MPress;
				}
				else
				{
					unsigned char* valChar= (unsigned char*)key.c_str();
					KeyBinds[setting.Name]= *valChar;
				}
				
			}
			//mouse inputs
				MouseBinds["Change_Gun_State_Up"] = Mouse::MouseEvent::EventType::WheelUp;
				MouseBinds["Change_Gun_State_Down"] = Mouse::MouseEvent::EventType::WheelDown;
				MouseBinds["Fire_Tool"] = Mouse::MouseEvent::EventType::LPress;

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
		Keyboard::KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

		// LEVEL SELECTION
		{
			if ( keycode == VK_NUMPAD1 ) levelSystem->SwitchTo( level_IDs[0] );
			if ( keycode == VK_NUMPAD2 ) levelSystem->SwitchTo( level_IDs[1] );
		}

		// CAMERA INPUT
		{
			// set camera to use
			if ( keycode == VK_F1 ) levelSystem->GetCurrentLevel()->GetCameraController()->SetIsUsingMain( true );
			if ( keycode == VK_F2 ) levelSystem->GetCurrentLevel()->GetCameraController()->SetIsUsingMain( false );

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
			
			// set multi-tool type
			
				if ( keycode == KeyBinds["Gun_State_One"]) {
					currentTool = ToolType::Convert;;
					EventSystem::Instance()->AddEvent(EVENTID::ChangeToolEvent, &currentTool);
				}
				if ( keycode == KeyBinds["Gun_State_Two"]) {
					currentTool = ToolType::Resize;
					EventSystem::Instance()->AddEvent(EVENTID::ChangeToolEvent, &currentTool);;
				}
				if (keycode == KeyBinds["Gun_State_Three"]) {
					
				}
				if (keycode == KeyBinds["Gun_State_Four"]) { 
					 }
				if (keycode == KeyBinds["Gun_State_Five"]) ;
				if (keycode == KeyBinds["Gun_State_Six"]) ;

				if (keycode == KeyBinds["Change_Gun_State_Up"]) {
					EventSystem::Instance()->AddEvent(EVENTID::ChangeToolOptionUpEvent);
				}
				if (keycode == KeyBinds["Change_Gun_State_Down"]) {
					EventSystem::Instance()->AddEvent(EVENTID::ChangeToolOptionDownEvent);
				}

		}
		

		//UI
		{
			UIChar = keycode;
			EventSystem::Instance()->AddEvent(EVENTID::UIKeyInput, &UIChar);


			if (keycode == KeyBinds["Pause"]) {
				// pause game
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
			cameras->GetCamera( levelSystem->GetCurrentLevel()->GetCameraController()->GetCurrentCamera() )->GetPositionFloat3() );

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
			if ( keyboard.KeyIsPressed(KeyBinds["Jump"]) || jumping )
				CameraMovement::Jump( cameras->GetCamera( JSON::CameraType::Default ), jumping, dt );
		}
	
		// normalize diagonal movement speed
		if ( keyboard.KeyIsPressed(KeyBinds["Forward"]) && ( keyboard.KeyIsPressed(KeyBinds["Left"]) || keyboard.KeyIsPressed(KeyBinds["Back"]) ) )
			cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.008f );
		if ( keyboard.KeyIsPressed(KeyBinds["Back"]) && ( keyboard.KeyIsPressed(KeyBinds["Left"]) || keyboard.KeyIsPressed(KeyBinds["Back"]) ) )
			cameras->GetCamera( cameras->GetCurrentCamera() )->SetCameraSpeed( 0.008f );

		// update camera movement
		if ( keyboard.KeyIsPressed(KeyBinds["Forward"]) ) CameraMovement::MoveForward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBinds["Left"]) ) CameraMovement::MoveLeft( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBinds["Back"]) ) CameraMovement::MoveBackward( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );
		if ( keyboard.KeyIsPressed(KeyBinds["Right"]) ) CameraMovement::MoveRight( cameras->GetCamera( cameras->GetCurrentCamera() ), playMode, dt );

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
				if ( i != j && levelSystem->GetCurrentLevel()->GetCube()[j]->GetIsHolding() == true )
					alreadyHeld = true;
      
			// pickup cube is in range, hovering with mouse and not already holding a cube - toggle function - was ( ( GetKeyState( KeyBindes["Action"] ) & 0x0001 ) != 0
			if ( ( keyboard.KeyIsPressed(KeyBinds["Action"] ) ) &&
					!alreadyHeld && levelSystem->GetCurrentLevel()->GetCube()[i]->GetIsInRange() && canHover &&
					( levelSystem->GetCurrentLevel()->GetCube()[i]->GetIsHovering() || levelSystem->GetCurrentLevel()->GetCube()[i]->GetIsHolding() ) )
			{
				levelSystem->GetCurrentLevel()->GetCube()[i]->SetIsHolding( true );

				// set cube position
				static int offset = 2;
				switch ( levelSystem->GetCurrentLevel()->GetCube()[i]->GetEditableProperties()->GetBoxSize() )
				{
				case BoxSize::Small:  offset = 1; break;
				case BoxSize::Normal: offset = 2; break;
				case BoxSize::Large:  offset = 4; break;
				}
				XMVECTOR cubePosition = cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionVector();
				cubePosition += cameras->GetCamera( cameras->GetCurrentCamera() )->GetForwardVector() * offset;
				levelSystem->GetCurrentLevel()->GetCube()[i]->SetPosition( cubePosition );

				// set cube rotation
				levelSystem->GetCurrentLevel()->GetCube()[i]->SetRotation(
					levelSystem->GetCurrentLevel()->GetCube()[i]->GetRotationFloat3().x,
					cameras->GetCamera( cameras->GetCurrentCamera() )->GetRotationFloat3().y,
					levelSystem->GetCurrentLevel()->GetCube()[i]->GetRotationFloat3().z
				);

				if (keyboard.KeyIsPressed('R'))
				{
					canHover = false;

					levelSystem->GetCurrentLevel()->GetCube()[i]->GetPhysicsModel()->AddForce(
						sinf(levelSystem->GetCurrentLevel()->GetCube()[i]->GetRotationFloat3().y) * 45.0f,
						-(cameras->GetCamera(cameras->GetCurrentCamera())->GetRotationFloat3().x + cameras->GetCamera(cameras->GetCurrentCamera())->GetRotationFloat3().z) / 2.0f * 100.0f,
						cosf(levelSystem->GetCurrentLevel()->GetCube()[i]->GetRotationFloat3().y) * 45.0f
					);
				}

			}

			else
			{
				levelSystem->GetCurrentLevel()->GetCube()[i]->SetIsHolding( false );
			}
		}

		if (!canHover && delay < 100.0f)
			delay += 1.0f;
		else
		{
			canHover = true;
			delay = 0.0f;
		}
	}

	// CUBE INPUT
	{
		// update cube movement
		if ( keyboard.KeyIsPressed( VK_RIGHT ) )
			levelSystem->GetCurrentLevel()->GetCube()[0]->GetPhysicsModel()->AddForce( 0.1f, 0.0f, 0.0f );
		if ( keyboard.KeyIsPressed( VK_LEFT ) )
			levelSystem->GetCurrentLevel()->GetCube()[0]->GetPhysicsModel()->AddForce( -0.1f, 0.0f, 0.0f );

		if ( keyboard.KeyIsPressed( 'R' ) )
		{
			//level->GetCube()[0]->AdjustPosition(
			//	cameras->GetCamera( cameras->GetCurrentCamera() )->GetForwardVector() *
			//	cameras->GetCamera( cameras->GetCurrentCamera() )->GetCameraSpeed() * dt );

			levelSystem->GetCurrentLevel()->GetCube()[0]->GetPhysicsModel()->AddForce(
				cameras->GetCamera( cameras->GetCurrentCamera() )->GetForwardVector()*
				cameras->GetCamera( cameras->GetCurrentCamera() )->GetCameraSpeed() * dt
			);
		}
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
			
			
			
					if (me.GetType() == MouseBinds["Change_Gun_State_Up"])
					{
						EventSystem::Instance()->AddEvent(EVENTID::ChangeToolOptionUpEvent);
					}
					else if (me.GetType() == MouseBinds["Change_Gun_State_Down"] )
					{
						EventSystem::Instance()->AddEvent(EVENTID::ChangeToolOptionDownEvent);
					}

			// mouse picking
			mousePick.UpdateMatrices( cameras->GetCamera( cameras->GetCurrentCamera() ) );
			for ( uint32_t i = 0; i < NUM_CUBES; i++ )
			{
				// testing sound, feel free to move or remove
				if ( me.GetType() == Mouse::MouseEvent::EventType::LPress )
				{
					if ( levelSystem->GetCurrentLevel()->GetLevelName() == "MainMenu" ||  isPaused )
						Sound::Instance()->PlaySoundEffect( "MenuClick" );
					else
						Sound::Instance()->PlaySoundEffect( "ToolUse" );
				}
				

				// test intersection between crosshair and cube
				if ( mousePick.TestIntersection( levelSystem->GetCurrentLevel()->GetGraphics()->GetWidth() / 2, levelSystem->GetCurrentLevel()->GetGraphics()->GetHeight() / 2, *levelSystem->GetCurrentLevel()->GetCube()[i] ) )
					levelSystem->GetCurrentLevel()->GetCube()[i]->SetIsHovering( true );
				else
					levelSystem->GetCurrentLevel()->GetCube()[i]->SetIsHovering( false );
#pragma region Tool_Picking
				
					// update box texture on click while hovering
					if (me.GetType() == MouseBinds["Fire_Tool"] && levelSystem->GetCurrentLevel()->GetCube()[i]->GetIsHovering())
					{
						//set 
						EventSystem::Instance()->AddEvent(EVENTID::ChangeCubeEvent, levelSystem->GetCurrentLevel()->GetCube()[i]->GetEditableProperties().get());
					}
#pragma endregion
			}
#pragma region UI_Input
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
#pragma endregion
		}
	}
}