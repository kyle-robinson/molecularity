#include "stdafx.h"
#include "Shaders.h"

// Vertex Shader Creation
HRESULT VertexShader::Initialize( Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements )
{
    // Compile the Vertex Shader
    HRESULT hr = CompileShaderFromFile( shaderPath.c_str(), "VS", "vs_5_0", shaderBuffer.GetAddressOf()  );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log(
            hr,
            "The FX file cannot be compiled.\n"
            "Please run this executable from the directory that contains the FX file."
        );
        return hr;
    }

	// Create the Vertex Shader
	hr = device->CreateVertexShader(
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        nullptr,
        shader.GetAddressOf()
    );
	if ( FAILED( hr ) )
	{
        ErrorLogger::Log( hr, "Failed to create Vertex Shader!" );
		shaderBuffer->Release();
        return hr;
	}

    // Create the Input Layout
	hr = device->CreateInputLayout(
        layoutDesc,
        numElements,
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        inputLayout.GetAddressOf()
    );
	shaderBuffer->Release();
	if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create Input Layout." );
        return hr;
    }

	return S_OK;
}

// Pixel Shader Creation
HRESULT PixelShader::Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath )
{
    // Compile the Pixel Shader
    HRESULT hr = CompileShaderFromFile( shaderPath.c_str(), "PS", "ps_5_0", shaderBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "The FX file cannot be compiled.\n"
            "Please run this executable from the directory that contains the FX file." );
        return hr;
    }

	// Create the Pixel Shader
	hr = device->CreatePixelShader(
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        nullptr,
        shader.GetAddressOf()
    );
	shaderBuffer->Release();
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create Pixel Shader!" );
        return hr;
    }

    return S_OK;
}

// Shader Compilation
HRESULT Shaders::CompileShaderFromFile( std::wstring szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    HRESULT hr = D3DCompileFromFile(
        szFileName.c_str(),
        nullptr,
        nullptr,
        szEntryPoint,
        szShaderModel,
        dwShaderFlags,
        0,
        ppBlobOut,
        &pErrorBlob
    );

    if ( FAILED( hr ) )
    {
        if ( pErrorBlob != nullptr )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );

        if ( pErrorBlob )
            pErrorBlob->Release();

        ErrorLogger::Log( hr, "Failed to compile shader from file!" );
        return hr;
    }

    if ( pErrorBlob )
        pErrorBlob->Release();

    return S_OK;
}

void Shaders::BindShaders( ID3D11DeviceContext* context, VertexShader& vs, PixelShader& ps ) noexcept
{
	context->VSSetShader( vs.GetShader(), NULL, 0 );
	context->IASetInputLayout( vs.GetInputLayout() );
	context->PSSetShader( ps.GetShader(), NULL, 0 );
}