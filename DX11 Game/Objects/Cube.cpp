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
    if ( !isHeld ) physicsModel->Update( deltaTime / 1000.0f );
}

#pragma region Collisions
void Cube::CheckCollisionAABB( GameObject& object, const float dt ) noexcept
{
    // adjust x/z collision scaling for pressure plate
    float collisionScale = 2.0f;
    if ( typeid( object ) == typeid( Cube ) )
        collisionScale = 0.0f;

    // test collision between cube and given object
    if ( ( position.x - GetScaleFloat3().x <= object.GetPositionFloat3().x + object.GetScaleFloat3().x + collisionScale && // x collision
           position.x + GetScaleFloat3().x >= object.GetPositionFloat3().x - object.GetScaleFloat3().x - collisionScale ) &&
         ( position.y - GetScaleFloat3().y <= object.GetPositionFloat3().y + object.GetScaleFloat3().y && // y collision
           position.y + GetScaleFloat3().y >= object.GetPositionFloat3().y - object.GetScaleFloat3().y ) &&
         ( position.z - GetScaleFloat3().z <= object.GetPositionFloat3().z + object.GetScaleFloat3().z + collisionScale && // z collision
           position.z + GetScaleFloat3().z >= object.GetPositionFloat3().z - object.GetScaleFloat3().z - collisionScale )
        )
    {
        // collsion with another cube
        if ( typeid( object ) == typeid( Cube ) )
        {
            CollisionResolution( dynamic_cast<Cube&>( object ), dt );
        }
        // collision with pressure plate
        else
        {
            position.y = object.GetPositionFloat3().y + object.GetScaleFloat3().y + 1.0f;
        }
        physicsModel->SetActivated( true );
    }
    else
    {
        physicsModel->SetActivated( false );
    }
}

void Cube::CollisionResolution( Cube& object, const float dt ) noexcept
{
    XMVECTOR v1 = XMVectorSet(
        physicsModel->GetNetForce().x,
        physicsModel->GetNetForce().y,
        physicsModel->GetNetForce().z, 1.0f
    );

    XMVECTOR v2 = XMVectorSet(
        object.GetPhysicsModel()->GetNetForce().x,
        object.GetPhysicsModel()->GetNetForce().y,
        object.GetPhysicsModel()->GetNetForce().z, 1.0f
    );
    
    float velocityOne = std::max( XMVectorGetX( XMVector3Length( v1 ) ), 1.0f );
    float velocityTwo = std::max( XMVectorGetX( XMVector3Length( v2 ) ), 1.0f );

    float forceMagnitude = ( physicsModel->GetMass() * velocityOne + object.GetPhysicsModel()->GetMass() * velocityTwo ) / dt;
    XMVECTOR force = XMVector3Normalize( object.GetPositionVector() - GetPositionVector() ) * forceMagnitude * 0.15f;

    XMFLOAT3 forceF = {
        XMVectorGetX( force ),
        XMVectorGetY( force ),
        XMVectorGetZ( force )
    };

    XMFLOAT3 forceF_Inv = { -forceF.x, -forceF.y, -forceF.z };
    
    physicsModel->AddForce( forceF_Inv );
    object.GetPhysicsModel()->AddForce( forceF );
}
#pragma endregion