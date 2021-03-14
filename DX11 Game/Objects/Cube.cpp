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
void Cube::CheckCollisionAABB( RenderableGameObject& object, const float dt ) noexcept
{
    if ( ( position.x - GetScaleFloat3().x <= object.GetPositionFloat3().x + object.GetScaleFloat3().x + 2.0f && // x collision
           position.x + GetScaleFloat3().x >= object.GetPositionFloat3().x - object.GetScaleFloat3().x - 2.0f ) &&
           position.y - GetScaleFloat3().y <= object.GetPositionFloat3().y + object.GetScaleFloat3().y && // y collision
         ( position.z - GetScaleFloat3().z <= object.GetPositionFloat3().z + object.GetScaleFloat3().z + 2.0f && // z collision
           position.z + GetScaleFloat3().z >= object.GetPositionFloat3().z - object.GetScaleFloat3().z - 2.0f )
        )
    {
        physicsModel->SetActivated( true );
        position.y = object.GetPositionFloat3().y + object.GetScaleFloat3().y + 1.0f;
    }
    else
    {
        physicsModel->SetActivated( false );
    }
}

void Cube::CollisionResolution( RenderableGameObject& object, const float dt ) noexcept
{
    physicsModel->SetVelocity( {
        physicsModel->GetVelocity().x,
        -physicsModel->GetVelocity().y,
        physicsModel->GetVelocity().x
    } );
}
#pragma endregion