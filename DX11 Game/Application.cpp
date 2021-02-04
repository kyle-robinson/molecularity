#include "Application.h"
#include "Input\\CameraMovement.h"

bool Application::Initialize(
	HINSTANCE hInstance,
	const std::string& windowTitle,
	const std::string& windowClass,
	int width,
	int height )
{
	timer.Start();

	if ( !renderWindow.Initialize( this, hInstance, windowTitle, windowClass, width, height ) )
		return false;

	if ( !gfx.Initialize( renderWindow.GetHWND(), width, height ) )
		return false;

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
		if ( mouse.IsRightDown() )
		{
			if ( me.GetType() == Mouse::MouseEvent::EventType::RawMove )
			{
				gfx.camera.AdjustRotation(
					XMFLOAT3(
						static_cast<float>( me.GetPosY() ) * 0.005f,
						static_cast<float>( me.GetPosX() ) * 0.005f,
						0.0f
					)
				);
			}
		}
	}

	// Update Game Input Here
	gfx.camera.SetCameraSpeed( 0.002f );
	if ( keyboard.KeyIsPressed( VK_SHIFT ) ) gfx.camera.SetCameraSpeed( 0.01f );
	if ( keyboard.KeyIsPressed( 'W' ) ) CameraMovement::MoveForward( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'A' ) ) CameraMovement::MoveLeft( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'S' ) ) CameraMovement::MoveBackward( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( 'D' ) ) CameraMovement::MoveRight( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( VK_SPACE ) ) CameraMovement::MoveUp( gfx.camera, dt );
	if ( keyboard.KeyIsPressed( VK_CONTROL ) ) CameraMovement::MoveDown( gfx.camera, dt );

	// Set Light Position
	XMVECTOR lightPosition = gfx.camera.GetPositionVector();
	lightPosition += gfx.camera.GetForwardVector() / 4;
	lightPosition += gfx.camera.GetRightVector() / 2;
	gfx.light.SetPosition( lightPosition );
	gfx.light.SetRotation( gfx.camera.GetRotationFloat3().x + XM_PI, gfx.camera.GetRotationFloat3().y, gfx.camera.GetRotationFloat3().z );

	gfx.Update( dt );
}

void Application::Render()
{
	gfx.BeginFrame();
	gfx.RenderFrame();
	gfx.EndFrame();
}