#include "Collisions.h"
#include "Camera.h"
#include "Cube.h"

bool Collisions::CheckCollisionCircle( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept
{
	if (( camera->GetPositionFloat3().x - object.GetPositionFloat3().x ) *
		( camera->GetPositionFloat3().x - object.GetPositionFloat3().x ) +
		( camera->GetPositionFloat3().z - object.GetPositionFloat3().z ) *
		( camera->GetPositionFloat3().z - object.GetPositionFloat3().z ) <= radius * radius )
		return true;
	else
		return false;
}

bool Collisions::CheckCollisionSphere( std::unique_ptr<Camera>& camera, GameObject3D& object, float radius ) noexcept
{
	if (( camera->GetPositionFloat3().x - object.GetPositionFloat3().x ) *
		( camera->GetPositionFloat3().x - object.GetPositionFloat3().x ) +
		( camera->GetPositionFloat3().y - object.GetPositionFloat3().y ) *
		( camera->GetPositionFloat3().y - object.GetPositionFloat3().y ) +
		( camera->GetPositionFloat3().z - object.GetPositionFloat3().z ) *
		( camera->GetPositionFloat3().z - object.GetPositionFloat3().z ) <= radius * radius )
		return true;
	else
		return false;
}

void Collisions::CheckCollisionLevel1( std::unique_ptr<Camera>& camera, GameObject3D& object, float offset ) noexcept
{
	if ( camera->GetPositionFloat3().z < -7.0f ) // entrance collisions
	{
		// X-COLLISIONS
		{
			if ( camera->GetPositionFloat3().x >= 2.5f )
				camera->SetPosition( 2.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

			if ( camera->GetPositionFloat3().x <= -2.5f )
				camera->SetPosition( -2.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );
		}

		// Z-COLLISIONS
		{
			if ( camera->GetPositionFloat3().z <= -20.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -20.0f );
		}
	}
	else // main area collisions
	{
		// X-COLLISIONS
		{
			if ( camera->GetPositionFloat3().x <= object.GetPositionFloat3().x - offset )
				camera->SetPosition( object.GetPositionFloat3().x - offset, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );
		
			if ( camera->GetPositionFloat3().x >= object.GetPositionFloat3().x + offset )
				camera->SetPosition( object.GetPositionFloat3().x + offset, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );
		}

		// Z-COLLISIONS
		{
			if ( camera->GetPositionFloat3().z >= 13.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, 13.0f );

			if ( camera->GetPositionFloat3().x >= 2.5f && camera->GetPositionFloat3().z <= -6.5f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.5f );

			if ( camera->GetPositionFloat3().x <= -2.5f && camera->GetPositionFloat3().z <= -6.5f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.5f );
		}
	}
}