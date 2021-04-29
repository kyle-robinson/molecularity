#include "stdafx.h"
#include "Level2.h"
#include "Rasterizer.h"

Level2::Level2( LevelStateMachine& stateMachine ) : levelStateMachine( stateMachine ) {}

bool Level2::OnCreate()
{
	try
	{
		/*if ( !initialized )
		{
			initialized = true;
			Level::Initialize( hWnd, camera, width, height );
		}*/

		// DRAWABLES
		{
			// models
			if ( !hubRoom.Initialize( "Resources\\Models\\Hub\\scene.gltf", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
			hubRoom.SetInitialScale( 4.0f, 4.0f, 4.0f );

			if ( !skysphere.Initialize( "Resources\\Models\\Sphere\\sphere.obj", device.Get(), context.Get(), cb_vs_matrix ) ) return false;
			skysphere.SetInitialScale( 250.0f, 250.0f, 250.0f );
		}
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void Level2::Render()
{
	// Render to sub viewport first using static camera
	this->GetMultiViewport()->SetUsingSub( true );
	this->BeginFrame();
	this->RenderFrame();

	// Render main scene next with main/debug camera
	this->GetMultiViewport()->SetUsingMain( true );
	this->BeginFrame();
	this->RenderFrame();

	// Render UI and present the complete frame
	this->EndFrame();
}

void Level2::RenderFrame()
{
	// SKYSPHERE
	{
		Shaders::BindShaders( context.Get(), vertexShader_light, pixelShader_noLight );
		GetRasterizer( "Skybox" )->Bind( *this );
		skysphere.Draw();
		GetRasterizer( rasterizerSolid ? "Solid" : "Wireframe" )->Bind( *this );
	}

	// LIGHTS
	{
		// w/out normals
		pointLight.Draw();
		directionalLight.Draw();

		// w/ normals
		context->PSSetShader( pixelShader_light.GetShader(), NULL, 0 );
		spotLight.Draw();
	}

	// DRAWABLES
	{
		hubRoom.Draw();
	}
}

void Level2::Update( const float dt )
{
	// update lights/skysphere
	pointLight.SetPosition( pointLight.GetLightPosition() );
	directionalLight.SetPosition( directionalLight.GetLightPosition() );
	skysphere.SetPosition( cameras->GetCamera( cameras->GetCurrentCamera() )->GetPositionFloat3() );

	// set position of spot light model
	spotLight.UpdateModelPosition( cameras->GetCamera( JSON::CameraType::Default ) );
}