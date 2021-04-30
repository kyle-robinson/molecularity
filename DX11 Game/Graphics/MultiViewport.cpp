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
		manager.GetGraphics()->GetViewport( "Sub" )->Bind( *manager.GetGraphics() );
		useViewportSub = false;
	}

	// Create view of main/debug camera and bind to main viewport.
	if ( useViewportMain )
	{
		if ( manager.GetCameraController()->GetIsUsingMain() )
			manager.GetCameraController()->SetCurrentCamera( JSON::CameraType::Default );
		else if ( !manager.GetCameraController()->GetIsUsingMain() )
			manager.GetCameraController()->SetCurrentCamera( JSON::CameraType::Debug );
		manager.GetGraphics()->GetViewport( "Main" )->Bind( *manager.GetGraphics() );
		useViewportMain = false;
	}
}