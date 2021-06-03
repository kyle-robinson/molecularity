#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Colour.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <assimp/material.h>

/// <summary>
/// Load texture information from model file when initializing data through Assimp library.
/// Provides default colour textures for models that fail to properly load their textures.
/// </summary>
enum class TextureStorageType
{
	None,
	Disk,
	Invalid,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
};

class Texture
{
public:
	Texture( ID3D11Device* device, const Colour& colour, aiTextureType type );
	Texture( ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type );
	Texture( ID3D11Device* device, const std::string& filePath, aiTextureType type );
	Texture( ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type );
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();

	void UpdateTexture( ID3D11Device* device, std::string file );
	void UpdateTexture( ID3D11Device* device, Colour& file );

private:
	void Initialize1x1ColourTexture( ID3D11Device* device, const Colour& colour, aiTextureType type );
	void InitializeColourTexture( ID3D11Device* device, const Colour* colorData, UINT width, UINT height, aiTextureType type );
private:
	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView = nullptr;
	aiTextureType type = aiTextureType_UNKNOWN;

	// For checking so not loaded evey frame :)
	std::string FileName;
	Colour ColourRGBA;
};

#endif