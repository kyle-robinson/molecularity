#include "stdafx.h"
#include "MultiViewport.h"
#include "Graphics.h"
#include "Viewport.h"

void MultiViewport::Update( Graphics& gfx )
{
	// Create view on static camera and bind to sub viewport.
	if ( useViewportSub )
	{
		gfx.GetCameraController()->SetCurrentCamera( JSON::CameraType::Static );
		gfx.GetViewport( "Sub" )->Bind( gfx );
		useViewportSub = false;
	}

	// Create view of main/debug camera and bind to main viewport.
	if ( useViewportMain )
	{
		if ( gfx.GetCameraController()->GetIsUsingMain() )
			gfx.GetCameraController()->SetCurrentCamera( JSON::CameraType::Default );
		else if ( !gfx.GetCameraController()->GetIsUsingMain() )
			gfx.GetCameraController()->SetCurrentCamera( JSON::CameraType::Debug );
		gfx.GetViewport( "Main" )->Bind( gfx );
		useViewportMain = false;
	}
}