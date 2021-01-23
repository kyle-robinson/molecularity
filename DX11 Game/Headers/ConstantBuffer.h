#pragma once
#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer( const ConstantBuffer<T>& rhs ) {}
public:
	T data;
	ConstantBuffer() {}
	ID3D11Buffer* Get() const noexcept { return buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf() const noexcept { return buffer.GetAddressOf(); }
	HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* context );
	bool ApplyChanges();
private:
	ID3D11DeviceContext* context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
};

#endif