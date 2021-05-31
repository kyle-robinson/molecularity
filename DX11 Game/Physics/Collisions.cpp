#include "Collisions.h"
#include "Camera.h"
#include "Cube.h"

#define RESET_FORCES cube->GetPhysicsModel()->ResetForces_NoY()

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

#pragma region Level1_Collisions
void Collisions::CheckCollisionLevel1( std::unique_ptr<Camera>& camera, GameObject3D& object, float offset ) noexcept
{
	if ( camera->GetPositionFloat3().z < -6.5f ) // entrance collisions
	{
		// X-COLLISIONS
		{
			if ( camera->GetPositionFloat3().x >= 4.5f )
				camera->SetPosition( 4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

			if ( camera->GetPositionFloat3().x <= -4.5f )
				camera->SetPosition( -4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );
		}

		// Z-COLLISIONS
		{
			if ( camera->GetPositionFloat3().z <= -23.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -23.0f );
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
			if ( camera->GetPositionFloat3().z >= 1.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, 1.0f );

			if ( camera->GetPositionFloat3().x >= 4.5f && camera->GetPositionFloat3().z <= -6.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.0f );

			if ( camera->GetPositionFloat3().x <= -4.5f && camera->GetPositionFloat3().z <= -6.0f )
				camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.0f );
		}
	}
}

void Collisions::CheckCollisionLevel1( std::shared_ptr<Cube>& cube, GameObject3D& object, float offset ) noexcept
{
	if ( cube->GetPositionFloat3().y <= 3.0f )
		cube->GetPhysicsModel()->CheckGroundCollisions( true );
	else
		cube->GetPhysicsModel()->CheckGroundCollisions( false );

	if ( cube->GetPositionFloat3().z < -6.5f ) // entrance collisions
	{
		// X-COLLISIONS
		{
			if ( cube->GetPositionFloat3().x >= 5.0f )
			{
				cube->SetPosition( 5.0f, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}

			if ( cube->GetPositionFloat3().x <= -5.0f )
			{
				cube->SetPosition( -5.0f, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}
		}

		// Z-COLLISIONS
		{
			if ( cube->GetPositionFloat3().z <= -23.0f )
			{
				cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -23.0f );
				RESET_FORCES;
			}
		}
	}
	else
	{
		// X-COLLISIONS
		{
			if ( cube->GetPositionFloat3().x <= object.GetPositionFloat3().x - offset )
			{
				cube->SetPosition( object.GetPositionFloat3().x - offset, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}

			if ( cube->GetPositionFloat3().x >= object.GetPositionFloat3().x + offset )
			{
				cube->SetPosition( object.GetPositionFloat3().x + offset, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}
		}

		// Z-COLLISIONS
		{
			if ( cube->GetPositionFloat3().z < 3.0f ) // main area collisions
			{
				if ( cube->GetPositionFloat3().x >= 5.0f && cube->GetPositionFloat3().z <= -6.0f )
				{
					cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -6.0f );
					RESET_FORCES;
				}

				if ( cube->GetPositionFloat3().x <= -5.0f && cube->GetPositionFloat3().z <= -6.0f )
				{
					cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -6.0f );
					RESET_FORCES;
				}
			}
			else if ( cube->GetPositionFloat3().z >= 3.0f && cube->GetPositionFloat3().z < 19.5f ) // sludge area collisions
			{
				cube->GetPhysicsModel()->CheckGroundCollisions( false );
				if ( cube->GetPositionFloat3().y <= -2.5f )
				{
					cube->ResetPosition();
					RESET_FORCES;
				}
			}
			else // pressure plate platform collisions
			{
				if ( cube->GetPositionFloat3().z >= 28.0f )
				{
					cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, 28.0f );
					RESET_FORCES;
				}
			}
		}
	}
}
#pragma endregion