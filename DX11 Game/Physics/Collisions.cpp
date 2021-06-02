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

void Collisions::CeilingCollision(std::shared_ptr<Cube>& cube, float ceilingHeight) noexcept
{
	if (cube->GetPositionFloat3().y > ceilingHeight)
	{
		cube->SetPosition(XMFLOAT3(cube->GetPositionFloat3().x, ceilingHeight, cube->GetPositionFloat3().z));
		cube->GetPhysicsModel()->CheckGroundCollisions(true);
		cube->GetPhysicsModel()->AddForce(XMFLOAT3(0.0f, -2.0f, 0.0f));
	}
}

#pragma region Level1_Collisions
void Collisions::CheckCollisionLevel1( std::unique_ptr<Camera>& camera, float offset ) noexcept
{
	if ( camera->GetPositionFloat3().z < -6.5f ) // entrance collisions
	{
		// X-COLLISIONS
		if ( camera->GetPositionFloat3().x >= 4.5f )
			camera->SetPosition( 4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		if ( camera->GetPositionFloat3().x <= -4.5f )
			camera->SetPosition( -4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		// Z-COLLISIONS
		if ( camera->GetPositionFloat3().z <= -23.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -23.0f );
	}
	else // main area collisions
	{
		// X-COLLISIONS
		if ( camera->GetPositionFloat3().x <= -offset + 1.5f )
			camera->SetPosition( -offset + 1.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );
		
		if ( camera->GetPositionFloat3().x >= offset - 1.5f )
			camera->SetPosition( offset - 1.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		// Z-COLLISIONS
		if ( camera->GetPositionFloat3().z >= 1.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, 1.0f );

		if ( camera->GetPositionFloat3().x >= 4.5f && camera->GetPositionFloat3().z <= -6.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.0f );

		if ( camera->GetPositionFloat3().x <= -4.5f && camera->GetPositionFloat3().z <= -6.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -6.0f );
	}
}

void Collisions::CheckCollisionLevel1( std::shared_ptr<Cube>& cube, float offset ) noexcept
{
	CeilingCollision( cube, 11.5f );
	cube->GetPhysicsModel()->CheckGroundCollisions( true );
	
	if ( cube->GetPositionFloat3().z < -6.5f ) // entrance collisions
	{
		// X-COLLISIONS
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

		// Z-COLLISIONS
		if ( cube->GetPositionFloat3().z <= -23.0f )
		{
			cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -23.0f );
			RESET_FORCES;
		}
	}
	else
	{
		// X-COLLISIONS
		if ( cube->GetPositionFloat3().x <= -offset - 1.5f )
		{
			cube->SetPosition( -offset - 1.5f, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
			RESET_FORCES;
		}

		if ( cube->GetPositionFloat3().x >= offset + 1.5f )
		{
			cube->SetPosition( offset + 1.5f, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
			RESET_FORCES;
		}

		// Z-COLLISIONS
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
				Sound::Instance()->PlaySoundEffect( "CubeSplash", false, cube->GetPositionFloat3(), 10.0f );
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
#pragma endregion

#pragma region Level2_Collisions
void Collisions::CheckCollisionLevel2( std::unique_ptr<Camera>& camera, float offset ) noexcept
{
	if ( camera->GetPositionFloat3().z < -7.5f ) // entrance collisions
	{
		// X-COLLISIONS
		if ( camera->GetPositionFloat3().x >= 4.5f )
			camera->SetPosition( 4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		if ( camera->GetPositionFloat3().x <= -4.5f )
			camera->SetPosition( -4.5f, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		// Z-COLLISIONS
		if ( camera->GetPositionFloat3().z <= -25.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -25.0f );
	}
	else // main area collisions
	{
		// X-COLLISIONS
		if ( camera->GetPositionFloat3().x <= -offset )
			camera->SetPosition( -offset, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		if ( camera->GetPositionFloat3().x >= offset )
			camera->SetPosition( offset, camera->GetPositionFloat3().y, camera->GetPositionFloat3().z );

		// Z-COLLISIONS
		if ( camera->GetPositionFloat3().z >= -1.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -1.0f );

		if ( camera->GetPositionFloat3().x >= 4.5f && camera->GetPositionFloat3().z <= -7.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -7.0f );

		if ( camera->GetPositionFloat3().x <= -4.5f && camera->GetPositionFloat3().z <= -7.0f )
			camera->SetPosition( camera->GetPositionFloat3().x, camera->GetPositionFloat3().y, -7.0f );
	}
}

void Collisions::CheckCollisionLevel2( std::shared_ptr<Cube>& cube, float offset ) noexcept
{
	CeilingCollision( cube, 11.5f );
	cube->GetPhysicsModel()->CheckGroundCollisions( true );
	
	// check floor collisions - don't check wall collisions if below the floor
	// (prevents cube jumping to floor level when colliding with the wall while in the sludge area)
	if ( ( cube->GetPositionFloat3().z >= 0.5f && cube->GetPositionFloat3().z < 26.5f ) && // center area collisions
		 ( ( cube->GetPositionFloat3().x > 9.5f || cube->GetPositionFloat3().x < -9.5f ) || // sludge area collisions
		   ( cube->GetPositionFloat3().z < 8.5f || cube->GetPositionFloat3().z > 18.5f ) ) )
	{
		cube->GetPhysicsModel()->CheckGroundCollisions( false );
		if ( cube->GetPositionFloat3().y < 0.5f )
			RESET_FORCES;
	}
	else
	{
		if ( cube->GetPositionFloat3().z < -7.5f ) // entrance collisions
		{
			// X-COLLISIONS
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

			// Z-COLLISIONS
			if ( cube->GetPositionFloat3().z <= -25.0f )
			{
				cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -25.0f );
				RESET_FORCES;
			}
		}
		else // main area
		{
			// X-COLLISIONS
			if ( cube->GetPositionFloat3().x <= -offset )
			{
				cube->SetPosition( -offset, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}

			if ( cube->GetPositionFloat3().x >= offset + 0.5f )
			{
				cube->SetPosition( offset + 0.5f, cube->GetPositionFloat3().y, cube->GetPositionFloat3().z );
				RESET_FORCES;
			}

			// Z-COLLISIONS
			if ( cube->GetPositionFloat3().z < 0.5f ) // first platform collisions
			{
				if ( cube->GetPositionFloat3().x >= 5.0f && cube->GetPositionFloat3().z <= -7.0f )
				{
					cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -7.0f );
					RESET_FORCES;
				}

				if ( cube->GetPositionFloat3().x <= -5.0f && cube->GetPositionFloat3().z <= -7.0f )
				{
					cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, -7.0f );
					RESET_FORCES;
				}
			}
			
			else if ( cube->GetPositionFloat3().z >= 35.0f ) // back wall collisions
			{
				cube->SetPosition( cube->GetPositionFloat3().x, cube->GetPositionFloat3().y, 35.0f );
				RESET_FORCES;
			}
		}
	}

	// if cube enters sludge, reset its position
	if ( cube->GetPositionFloat3().y <= -2.5f )
	{
		Sound::Instance()->PlaySoundEffect( "CubeSplash", false, cube->GetPositionFloat3(), 10.0f );
		cube->ResetPosition();
		RESET_FORCES;
	}
}
#pragma endregion