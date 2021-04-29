#include "stdafx.h"
#include "MultiViewport.h"
#include "Viewport.h"
#include "Level.h"

void MultiViewport::Update( Level& manager )
{
	// Create view on static camera and bind to sub viewport.
	if ( useViewportSub )
	{
		manager.GetCameraController()->SetCurrentCamera( JSON::CameraType::Static );
		manager.GetViewport( "Sub" )->Bind( manager );
		useViewportSub = false;
	}

	// Create view of main/debug camera and bind to main viewport.
	if ( useViewportMain )
	{
		if ( manager.GetCameraController()->GetIsUsingMain() )
			manager.GetCameraController()->SetCurrentCamera( JSON::CameraType::Default );
		else if ( !manager.GetCameraController()->GetIsUsingMain() )
			manager.GetCameraController()->SetCurrentCamera( JSON::CameraType::Debug );
		manager.GetViewport( "Main" )->Bind( manager );
		useViewportMain = false;
	}
}