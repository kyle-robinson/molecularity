#include "stdafx.h"
#include "Credits_Level.h"

Credits_Level::Credits_Level( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Credits_Level::OnCreate()
{
	try
	{
		// User Interface
		credits = std::make_shared<Credits_UI>();
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Credits_Level::OnSwitch()
{
	// Update Level System
	levelName = "Credits";
	numOfCubes = 0;
	LevelContainer::UpdateCubes();

	// Update UI System
	_UiManager->RemoveAllUI();
	_UiManager->AddUi( credits, "Credits" );
	_UiManager->Initialize( graphics->device.Get(), graphics->context.Get(), &cb_vs_matrix_2d );

	// Update Sound System
	Sound::Instance()->InitialiseMusicTrack( "Credits" );
	Sound::Instance()->PlayMusic( "Credits" );
}

void Credits_Level::Render()
{
	// Render to sub viewport first using static camera
	GetMultiViewport()->SetUsingSub();
	BeginFrame();
	RenderFrame();

	// Render main scene next with main/debug camera
	GetMultiViewport()->SetUsingMain();
	BeginFrame();
	RenderFrame();

	// Render UI and present the complete frame
	EndFrame();
}
void Credits_Level::RenderFrame()
{
	// Render lights/skysphere
	LevelContainer::RenderFrameEarly();
	// Render the cubes
	LevelContainer::RenderFrame();

	if ( cameras->GetCurrentCamera() != JSON::CameraType::Static )
	{
		Shaders::BindShaders( graphics->context.Get(), graphics->vertexShader_2D, graphics->pixelShader_2D );
		cb_ps_scene.data.useTexture = TRUE;
		if ( !cb_ps_scene.ApplyChanges() ) return;
		graphics->context->PSSetConstantBuffers( 1u, 1u, cb_ps_scene.GetAddressOf() );
	}
}
void Credits_Level::Update( const float dt )
{
	// Update lights/skysphere
	LevelContainer::Update( dt );

	// Update cubes/multi-tool position
	LevelContainer::LateUpdate(dt);
}

void Credits_Level::CleanUp()
{
}
