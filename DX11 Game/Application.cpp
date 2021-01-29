#include "Application.h"

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
	float camera3DSpeed = 0.002f;

	if ( keyboard.KeyIsPressed( VK_SHIFT ) )
		camera3DSpeed = 0.01f;

	if ( keyboard.KeyIsPressed( 'W' ) )
		gfx.camera.AdjustPosition( gfx.camera.GetForwardVector() * camera3DSpeed * dt );

	if ( keyboard.KeyIsPressed( 'A' ) )
		gfx.camera.AdjustPosition( gfx.camera.GetLeftVector() * camera3DSpeed * dt );

	if ( keyboard.KeyIsPressed( 'S' ) )
		gfx.camera.AdjustPosition( gfx.camera.GetBackwardVector() * camera3DSpeed * dt );

	if ( keyboard.KeyIsPressed( 'D' ) )
		gfx.camera.AdjustPosition( gfx.camera.GetRightVector() * camera3DSpeed * dt );

	if ( keyboard.KeyIsPressed( VK_SPACE ) )
		gfx.camera.AdjustPosition( XMFLOAT3( 0.0f, camera3DSpeed * dt, 0.0f ) );

	if ( keyboard.KeyIsPressed( 'E' ) )
		gfx.camera.AdjustPosition( XMFLOAT3( 0.0f, -camera3DSpeed * dt, 0.0f ) );

	if ( keyboard.KeyIsPressed( 'C' ) )
	{
		XMVECTOR lightPosition = gfx.camera.GetPositionVector();
		lightPosition += gfx.camera.GetForwardVector();
		gfx.light.SetPosition( lightPosition );
		gfx.light.SetRotation( gfx.camera.GetRotationFloat3() );
	}

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