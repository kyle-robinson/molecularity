#include "Quad.h"
#include "Indices.h"
#include "Vertices.h"

// NORMAL QUAD
bool Quad::Initialize( ID3D11DeviceContext* context, ID3D11Device* device )
{
    this->context = context;

    try
    {
        HRESULT hr = vb_plane.Initialize( device, Vtx::verticesQuad, ARRAYSIZE( Vtx::verticesQuad ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create quad vertex buffer!" );
        hr = ib_plane.Initialize( device, Idx::indicesQuad, ARRAYSIZE( Idx::indicesQuad ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create quad index buffer!" );

        SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
        SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
        SetScale( 1.0f, 1.0f );
        UpdateMatrix();
    }
    catch ( COMException& exception )
    {
        ErrorLogger::Log( exception );
        return false;
    }

    return true;
}

void Quad::Draw( ConstantBuffer<CB_VS_matrix>& cb_vs_matrix, ConstantBuffer<CB_PS_scene>& cb_ps_scene,
    ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* textureNormal ) noexcept
{
    UINT offset = 0u;
    context->IASetVertexBuffers( 0u, 1u, vb_plane.GetAddressOf(), vb_plane.StridePtr(), &offset );
    context->IASetIndexBuffer( ib_plane.Get(), DXGI_FORMAT_R16_UINT, 0u );

    context->PSSetShaderResources( 0u, 1u, &texture );
    context->PSSetShaderResources( 1u, 1u, &textureNormal );

    cb_vs_matrix.data.worldMatrix = XMMatrixIdentity() * worldMatrix;
    if ( !cb_vs_matrix.ApplyChanges() ) return;
    context->VSSetConstantBuffers( 0u, 1u, cb_vs_matrix.GetAddressOf() );

    cb_ps_scene.data.useNormalMap = TRUE;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );

    context->DrawIndexed( ib_plane.IndexCount(), 0u, 0u );

    cb_ps_scene.data.useNormalMap = FALSE;
	if ( !cb_ps_scene.ApplyChanges() ) return;
	context->PSSetConstantBuffers( 2u, 1u, cb_ps_scene.GetAddressOf() );
}

// FULLSCREEN QUAD
bool QuadFullscreen::Initialize( ID3D11Device* device )
{
    try
    {
        HRESULT hr = vb_full.Initialize( device, Vtx::verticesFullscreen, ARRAYSIZE( Vtx::verticesFullscreen ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create fullscreen quad vertex buffer!" );
        hr = ib_full.Initialize( device, Idx::indicesFullscreen, ARRAYSIZE( Idx::indicesFullscreen ) );
        COM_ERROR_IF_FAILED( hr, "Failed to create fullscreen quad index buffer!" );
    }
    catch ( COMException& exception )
    {
        ErrorLogger::Log( exception );
        return false;
    }

    return true;
}

void QuadFullscreen::SetupBuffers( ID3D11DeviceContext* context,
    ConstantBuffer<CB_VS_fullscreen>& cb_vs_full, BOOL multiView ) noexcept
{
    UINT offset = 0u;
    context->IASetVertexBuffers( 0u, 1u, vb_full.GetAddressOf(), vb_full.StridePtr(), &offset );
    context->IASetIndexBuffer( ib_full.Get(), DXGI_FORMAT_R16_UINT, 0u );
    cb_vs_full.data.multiView = multiView;
    if ( !cb_vs_full.ApplyChanges() ) return;
    context->VSSetConstantBuffers( 0u, 1u, cb_vs_full.GetAddressOf() );
}