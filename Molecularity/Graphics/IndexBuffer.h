#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

/// <summary>
/// Creates an IndexBuffer object to store the indices for a game object.
/// Must be initialized which a struct which contains the appropriate indices.
/// </summary>
class IndexBuffer
{
private:
	IndexBuffer( const IndexBuffer& rhs ) { UNREFERENCED_PARAMETER( rhs ); }
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT indexCount = 0;
public:
	IndexBuffer() {}
	ID3D11Buffer* Get() const noexcept { return buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const noexcept { return buffer.GetAddressOf(); }
	UINT IndexCount() const noexcept { return indexCount; }
	HRESULT Initialize( ID3D11Device* device, WORD* data, UINT indexCount )
	{
		if ( buffer.Get() != nullptr )
			buffer.Reset();
		
		this->indexCount = indexCount;

		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof( WORD ) * indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		indexBufferData.pSysMem = data;

		HRESULT hr = device->CreateBuffer( &indexBufferDesc, &indexBufferData, buffer.GetAddressOf() );
		return hr;
	}
};

#endif