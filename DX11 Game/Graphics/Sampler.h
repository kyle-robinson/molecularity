#pragma once
#ifndef SAMPLER_H
#define SAMPLER_H

#include "GraphicsResource.h"
#include "ErrorLogger.h"

namespace Bind
{
	/// <summary>
	/// Creates a sampler which changes the method used to sample texture colours in the pixel shader.
	/// Constructor takes a sampler type, which means multiple samplers can be created which used different sampling methods.
	/// Call "Bind( gfx )" on a class object to bind it to the render pipeline.
	/// </summary>
	class Sampler : public GraphicsResource
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point
		};
		Sampler( GraphicsContainer& gfx, Type type, bool reflect = false, UINT slot = 0u )
			: type( type ), reflect( reflect ), slot( slot )
		{
			try
			{
				std::string samplerType;

				CD3D11_SAMPLER_DESC samplerDesc( CD3D11_DEFAULT{} );
				samplerDesc.Filter = [type, samplerType]() mutable {
					switch ( type )
					{
					case Type::Anisotropic:
						samplerType = "Anisotropic";
						return D3D11_FILTER_ANISOTROPIC;
					case Type::Point:
						samplerType = "Point";
						return D3D11_FILTER_MIN_MAG_MIP_POINT;
					default:
					case Type::Bilinear:
						samplerType = "Bilinear";
						return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
					}
				}();
				samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

				HRESULT hr = GetDevice( gfx )->CreateSamplerState( &samplerDesc, pSampler.GetAddressOf() );
				COM_ERROR_IF_FAILED( hr, "Failed to create sampler state!" );
			}
			catch ( COMException& exception )
			{
				ErrorLogger::Log( exception );
				return;
			}
		}
		void Bind( GraphicsContainer& gfx ) noexcept override
		{
			GetContext( gfx )->PSSetSamplers( slot, 1u, pSampler.GetAddressOf() );
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		bool reflect;
		Type type;
		UINT slot;
	};
}

#endif