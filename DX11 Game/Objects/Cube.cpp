#include "Cube.h"
#include "Indices.h"
#include "Vertices.h"

bool Cube::Initialize( ID3D11DeviceContext* context, ID3D11Device* device )
{
    this->context = context;
   
    try
    {
        HRESULT hr = vb_cube.Initialize( device, Vtx::verticesCube, ARRAYSIZE( Vtx::verticesCube ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create cube vertex buffer!" );
        hr = ib_cube.Initialize( device, Idx::indicesCube, ARRAYSIZE( Idx::indicesCube ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create cube index buffer!" );
        
        editableProperties = std::make_shared<CubeProperties>();
        physicsModel = std::make_shared<PhysicsModel>( this );

       
        
        SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	    SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
        SetScale( 1.0f, 1.0f, 1.0f );
	    UpdateMatrix();
    }
    catch ( COMException& exception )
    {
        ErrorLogger::Log( exception );
        return false;
    }
    
    delay = 0;

    return true;
}

void Cube::Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ID3D11ShaderResourceView* texture ) noexcept
{
	UINT offset = 0;
    context->IASetVertexBuffers( 0, 1, vb_cube.GetAddressOf(), vb_cube.StridePtr(), &offset );
    context->IASetIndexBuffer( ib_cube.Get(), DXGI_FORMAT_R16_UINT, 0 );
    context->PSSetShaderResources( 0, 1, &texture );
    cb_vs_matrix.data.worldMatrix = XMMatrixIdentity() * worldMatrix;
    if ( !cb_vs_matrix.ApplyChanges() ) return;
    context->VSSetConstantBuffers( 0, 1, cb_vs_matrix.GetAddressOf() );
    context->DrawIndexed( ib_cube.IndexCount(), 0, 0 );
}

void Cube::Update( const float deltaTime ) noexcept
{
    if ( !isHeld ) physicsModel->Update( deltaTime / 20.0f );

    else 
        physicsModel->Update(deltaTime / 20.0f, true);

    pos = GetPositionFloat3();

    if (heldLastFrame && !isHeld && (pos.x != prevPos.x || pos.z != prevPos.z))
        physicsModel->AddForce(XMFLOAT3((pos.x - prevPos.x) * 5.0f, 0.0f, (pos.z - prevPos.z) * 5.0f));

    if (delay == 5)
        prevPos = pos;
    delay++;
    if (delay > 5)
        delay = 0;
    heldLastFrame = isHeld;
}

#pragma region Collisions
void Cube::CheckCollisionAABB( RenderableGameObject& object, const float dt ) noexcept
{
    static float offset = 2.0f;

    // test collision between cube and given object
    if ( ( position.x - GetScaleFloat3().x <= object.GetPositionFloat3().x + object.GetScaleFloat3().x + offset && // x collision
           position.x + GetScaleFloat3().x >= object.GetPositionFloat3().x - object.GetScaleFloat3().x - offset ) &&
         ( position.y - GetScaleFloat3().y <= object.GetPositionFloat3().y + object.GetScaleFloat3().y && // y collision
           position.y + GetScaleFloat3().y >= object.GetPositionFloat3().y - object.GetScaleFloat3().y ) &&
         ( position.z - GetScaleFloat3().z <= object.GetPositionFloat3().z + object.GetScaleFloat3().z + offset && // z collision
           position.z + GetScaleFloat3().z >= object.GetPositionFloat3().z - object.GetScaleFloat3().z - offset )
        )
    {
        // collision with pressure plate
        position.y = object.GetPositionFloat3().y + object.GetScaleFloat3().y + 1.0f;
        physicsModel->SetActivated( true );
    }
    else
    {
        physicsModel->SetActivated( false );
    }
}

void Cube::CheckCollisionAABB( std::shared_ptr<Cube>& object, const float dt ) noexcept
{
    // adjust x/z collision scaling for pressure plate
    static float offset = 0.5f;

    // test collision between cube and given object
    if ( ( position.x - offset <= object->GetPositionFloat3().x + offset && // x collision
           position.x + offset >= object->GetPositionFloat3().x - offset ) &&
         ( position.y - offset <= object->GetPositionFloat3().y + offset && // y collision
           position.y + offset >= object->GetPositionFloat3().y - offset ) &&
         ( position.z - offset <= object->GetPositionFloat3().z + offset && // z collision
           position.z + offset >= object->GetPositionFloat3().z - offset )
        )
    {
        CollisionResolution( object, dt );
    }
}

void Cube::CollisionResolution( std::shared_ptr<Cube>& object, const float dt ) noexcept
{
    float velocityOne = std::max( physicsModel->Magnitude( physicsModel->GetNetForce() ), 1.0f );
    float velocityTwo = std::max( object->GetPhysicsModel()->Magnitude( object->GetPhysicsModel()->GetNetForce() ), 1.0f );

    float forceMagnitude = ( physicsModel->GetMass() * velocityOne + object->GetPhysicsModel()->GetMass() * velocityTwo ) / dt;
    XMFLOAT3 force;
    force.x = object->GetPositionFloat3().x - GetPositionFloat3().x;
    force.y = object->GetPositionFloat3().y - GetPositionFloat3().y;
    force.z = object->GetPositionFloat3().z - GetPositionFloat3().z;

    force.x = physicsModel->Normalization( force ).x * forceMagnitude * 0.015f;
    force.y = physicsModel->Normalization( force ).y * forceMagnitude * 0.175f;
    force.z = physicsModel->Normalization( force ).z * forceMagnitude * 0.015f;

    XMFLOAT3 force2 = {
        -force.x,
        -force.y,
        -force.z,
    };

    physicsModel->AddForce( force2 );
    object->GetPhysicsModel()->AddForce( force );
}
#pragma endregion