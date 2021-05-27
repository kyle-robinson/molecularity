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
    // update properties
    switch ( editableProperties->GetBoxType() )
    {
    case BoxType::Mesh:
        physicsModel->SetMass( 10.0f );
        physicsModel->SetBounciness( 0.0f );
       
        editableProperties->SetMagnetic( true );
        editableProperties->SetOutlineColor( { 0.18f, 0.8f, 0.44f } );
        break;
    case BoxType::Wood:
        physicsModel->SetMass( 30.0f );
        physicsModel->SetBounciness( 0.0f );
  
        editableProperties->SetMagnetic( false );
        editableProperties->SetOutlineColor( { 0.9f, 0.49f, 0.13f } );
        break;
    case BoxType::Stone:
        physicsModel->SetMass( 50.0f );
        physicsModel->SetBounciness( 0.0f );
       
        editableProperties->SetMagnetic( false );
        editableProperties->SetOutlineColor( { 0.2f, 0.59f, 0.85f } );
        break;
    case BoxType::Iron:
        physicsModel->SetMass( 70.0f );
        physicsModel->SetBounciness( 0.0f );
        
        editableProperties->SetMagnetic( true );
        editableProperties->SetOutlineColor( { 0.6f, 0.34f, 0.71f } );
        break;
    case BoxType::Alien:
        physicsModel->SetMass( 100.0f );
        physicsModel->SetBounciness( 0.5f );
      
        editableProperties->SetMagnetic( true );
        editableProperties->SetOutlineColor( { 0.9f, 0.29f, 0.23f } );
        break;
    }

    // update sizing
    switch ( editableProperties->GetBoxSize() )
    {
    case BoxSize::Small:  physicsModel->SetMass( physicsModel->GetMass() + 10.0f ); break;
    case BoxSize::Normal: physicsModel->SetMass( physicsModel->GetMass() + 25.0f ); break;
    case BoxSize::Large:  physicsModel->SetMass( physicsModel->GetMass() + 50.0f ); break;
    }

    // update bounciness
    switch ( editableProperties->GetBoxBounce() )
    {
    case BoxBounce::Solid: physicsModel->InvVelocity( false ); break;
    case BoxBounce::Bouncy: physicsModel->InvVelocity( true ); break;
    }

    //Magnetic pull
    MagneticForce();

    // update physics
    if (!isHeld) {
      
        physicsModel->Update(deltaTime / 20.0f, editableProperties);
    }
    else
        physicsModel->Update( deltaTime / 20.0f, editableProperties, true );

    // update positioning
    pos = GetPositionFloat3();

    if (heldLastFrame && !isHeld && (pos.x != prevPos.x || pos.z != prevPos.z)) {
        physicsModel->AddForce(XMFLOAT3((pos.x - prevPos.x) * 5.0f, 0.0f, (pos.z - prevPos.z) * 5.0f));
    }

    if ( delay == 5 )
        prevPos = pos;
    else if ( delay > 5 )
        delay = 0;

    delay++;

    heldLastFrame = isHeld;
}

#pragma region Collisions
bool Cube::CheckCollisionAABB( RenderableGameObject& object, const float dt ) noexcept
{
    // set collision offset
    static float offset = 1.0f;
    switch ( editableProperties->GetBoxSize() )
    {
    case BoxSize::Small:  offset = 0.75f; break;
    case BoxSize::Normal: offset = 1.0f;  break;
    case BoxSize::Large:  offset = 1.5f;  break;
    default: break;
    }

    // test collision between cube and given object
    if ( ( position.x - GetScaleFloat3().x <= object.GetPositionFloat3().x + object.GetScaleFloat3().x + offset &&  // x collision
           position.x + GetScaleFloat3().x >= object.GetPositionFloat3().x - object.GetScaleFloat3().x - offset ) &&
         ( position.y - GetScaleFloat3().y <= object.GetPositionFloat3().y + object.GetScaleFloat3().y + offset &&  // y collision
           position.y + GetScaleFloat3().y >= object.GetPositionFloat3().y - object.GetScaleFloat3().y - offset ) &&
         ( position.z - GetScaleFloat3().z <= object.GetPositionFloat3().z + object.GetScaleFloat3().z + offset &&  // z collision
           position.z + GetScaleFloat3().z >= object.GetPositionFloat3().z - object.GetScaleFloat3().z - offset )
        )
    {
        // collision with pressure plate
        position.y = object.GetPositionFloat3().y + object.GetScaleFloat3().y + offset;
        physicsModel->SetActivated( true );
        return true;
    }
    else
    {
        physicsModel->SetActivated( false );
        return false;
    }
}

void Cube::CheckCollisionAABB( std::shared_ptr<Cube>& object, const float dt ) noexcept
{
    // set collision offset
    static float offset = 0.5f;
    switch ( editableProperties->GetBoxSize() )
    {
    case BoxSize::Small:  offset = -0.25f; break;
    case BoxSize::Normal: offset = 0.5f;   break;
    case BoxSize::Large:  offset = 0.75f;  break;
    default: break;
    }

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
    
    Sound::Instance()->PlaySoundEffect( "CubeCollision", false, GetPositionFloat3(), 10.0f );
}
void Cube::MagneticForce()
{
    if (editableProperties->GetBoxMagneticMove()) {
        if (!cubeInRange) {
            XMFLOAT3 force = physicsModel->Normalization(XMFLOAT3((CamPos.x - pos.x), (CamPos.y - pos.y), (CamPos.z - pos.z)));
            
            physicsModel->AddForce(XMFLOAT3{ force.x * MagPower,force.y * MagPower,force.z * MagPower });
            physicsModel->UseWeight(false);
        }
        else {
            physicsModel->ResetForces();
            editableProperties->SetBoxMagneticMove(false);
            physicsModel->UseWeight(true);
        }
    }
}
#pragma endregion