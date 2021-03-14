#include "Sprite.h"
#include <dxtk/WICTextureLoader.h>

bool Sprite::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	float width, float height, std::string spritePath,
	ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d )
{
	this->context = context;
	if ( context == nullptr ) return false;

	this->cb_vs_matrix_2d = &cb_vs_matrix_2d;
	texture = std::make_unique<Texture>( device, spritePath, aiTextureType_DIFFUSE );

	std::vector<Vertex2D> vertexData =
	{
		{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f } },
		{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f } },
		{ {  0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f } },
	};

	std::vector<WORD> indexData =
	{
		0, 1, 2,
		2, 1, 3
	};

	HRESULT hr = vertices.Initialize( device, vertexData.data(), static_cast<UINT>( vertexData.size() ) );
	COM_ERROR_IF_FAILED( hr, "Failed to initialize vertices for sprite!" );

	hr = indices.Initialize( device, indexData.data(), static_cast<UINT>( indexData.size() ) );
	COM_ERROR_IF_FAILED( hr, "Failed to initialize indices for sprite!" );

	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetScale( width, height );

	return true;
}

//colour texture initalize
bool Sprite::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, float width, float height, Colour spriteColour, ConstantBuffer<CB_VS_matrix_2D>& cb_vs_matrix_2d)
{
	this->context = context;
	if (context == nullptr) return false;

	this->cb_vs_matrix_2d = &cb_vs_matrix_2d;
	
	texture = std::make_unique<Texture>(device, spriteColour, aiTextureType_DIFFUSE);

	std::vector<Vertex2D> vertexData =
	{
		{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f } },
		{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 1.0f } },
		{ {  0.5f,  0.5f, 0.0f },{ 1.0f, 1.0f } },
	};

	std::vector<WORD> indexData =
	{
		0, 1, 2,
		2, 1, 3
	};

	HRESULT hr = vertices.Initialize(device, vertexData.data(), static_cast<UINT>(vertexData.size()));
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertices for sprite!");

	hr = indices.Initialize(device, indexData.data(), static_cast<UINT>(indexData.size()));
	COM_ERROR_IF_FAILED(hr, "Failed to initialize indices for sprite!");

	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(width, height);
	return true;
}

void Sprite::Draw( XMMATRIX orthoMatrix )
{
	XMMATRIX wvpMatrix = worldMatrix * orthoMatrix;
	context->VSSetConstantBuffers( 0, 1, cb_vs_matrix_2d->GetAddressOf() );
	cb_vs_matrix_2d->data.wvpMatrix = wvpMatrix;
	cb_vs_matrix_2d->ApplyChanges();
	context->PSSetShaderResources( 0, 1, texture->GetTextureResourceViewAddress() );

	const UINT offsets = 0; 
	context->IASetVertexBuffers( 0, 1, vertices.GetAddressOf(), vertices.StridePtr(), &offsets );
	context->IASetIndexBuffer( indices.Get(), DXGI_FORMAT_R16_UINT, 0 );
	context->DrawIndexed( indices.IndexCount(), 0, 0 );
}

void Sprite::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling( scale.x, scale.y, 1.0f ) *
		XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z ) *
		XMMatrixTranslation( position.x + scale.x / 2.0f, position.y + scale.y / 2.0f, position.z );
}