#pragma once
#ifndef SHADERS_H
#define SHADERS_H

class VertexShader;
class PixelShader;

/// <summary>
/// Handles the loading of vertex and pixel shaders from file.
/// Call "Shaders::BindShaders( vertexShader, pixelShader )" to bind appropriate shaders to the pipeline.
/// </summary>
class Shaders
{
public:
	static void BindShaders( ID3D11DeviceContext* context, VertexShader& vs, PixelShader& ps ) noexcept;
protected:
	HRESULT CompileShaderFromFile(
		std::wstring szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut );
};

/// <summary>
/// Initialize a vertex shader given a filepath and input layout.
/// Can be bound to the pipeline using the Shaders class.
/// </summary>
class VertexShader : Shaders
{
public:
	HRESULT Initialize(
		Microsoft::WRL::ComPtr<ID3D11Device> &device,
		std::wstring shaderPath,
		D3D11_INPUT_ELEMENT_DESC* layoutDesc,
		UINT numElements
	);
	ID3D11VertexShader* GetShader() const noexcept { return shader.Get(); }
	ID3D10Blob* GetBuffer() const noexcept { return shaderBuffer.Get(); }
	ID3D11InputLayout* GetInputLayout() const noexcept { return inputLayout.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
};

/// <summary>
/// Initialize a pixel shader given a filepath.
/// Can be bound to the pipeline using the Shaders class.
/// </summary>
class PixelShader : Shaders
{
public:
	HRESULT Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath );
	ID3D11PixelShader* GetShader() const noexcept { return shader.Get(); }
	ID3D10Blob* GetBuffer() const noexcept { return shaderBuffer.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
};

#endif