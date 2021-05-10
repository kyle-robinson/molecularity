#include "StencilOutline.h"
#include "RenderableGameObject.h"
#include "InputLayout.h"
#include "Cube.h"
#include <imgui/imgui.h>

StencilOutline::StencilOutline( Graphics& gfx ) : scale( 0.1f ), color( XMFLOAT3( 1.0f, 0.6f, 0.1f ) )
{			
	try
	{				
		HRESULT hr = cb_ps_outline.Initialize( GetDevice( gfx ), GetContext( gfx ) );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'cb_ps_outline' constant buffer!" );

		hr = vertexShader_outline.Initialize( GetDeviceRef( gfx ), L"Resources\\Shaders\\Model_Outline.fx",
			Layout::layoutPosCol, ARRAYSIZE( Layout::layoutPosCol ) );
		hr = pixelShader_outline.Initialize( GetDeviceRef( gfx ), L"Resources\\Shaders\\Model_Outline.fx" );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'Outline' shaders!" );

	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return;
	}
}

void StencilOutline::DrawWithOutline( Graphics& gfx, Cube& cube, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix,
	ConstantBuffer<CB_PS_point>& cb_ps_point, ID3D11ShaderResourceView* texture )
{
	// write pixels to the buffer, which will act as the stencil mask
	GetStencil( gfx, "Write" )->Bind( gfx );
	cube.Draw( cb_vs_matrix, texture );

	// scale the cube and draw the stencil outline, ignoring the pixels previously written to the buffer
	Shaders::BindShaders( GetContext( gfx ), vertexShader_outline, pixelShader_outline );
	cb_ps_outline.data.outlineColor = color;
	if ( !cb_ps_outline.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 6u, 1u, cb_ps_outline.GetAddressOf() );
	GetStencil( gfx, "Mask" )->Bind( gfx );
	cube.SetScale(
		cube.GetScaleFloat3().x + scale,
		cube.GetScaleFloat3().y + scale,
		cube.GetScaleFloat3().z + scale
	);
	cube.Draw( cb_vs_matrix, texture );

	// rescale the cube and draw using the appropriate shaders and textures
	Shaders::BindShaders( GetContext( gfx ), gfx.vertexShader_light, gfx.pixelShader_light );
	if ( !cb_ps_point.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	cube.SetScale(
		cube.GetScaleFloat3().x - scale,
		cube.GetScaleFloat3().y - scale,
		cube.GetScaleFloat3().z - scale
	);
	GetStencil( gfx, "Off" )->Bind( gfx );
	cube.Draw( cb_vs_matrix, texture );
}

void StencilOutline::DrawWithOutline( Graphics& gfx, RenderableGameObject& object, ConstantBuffer<CB_PS_point>& cb_ps_point )
{
	static XMFLOAT3 outlineColor = { 0.0f, 0.29f, 0.85f };

	// write pixels to the buffer, which will act as the stencil mask
	GetStencil( gfx, "Write" )->Bind( gfx );
	object.Draw();

	// scale the model and draw the stencil outline, ignoring the pixels previously written to the buffer
	Shaders::BindShaders( GetContext( gfx ), vertexShader_outline, pixelShader_outline );
	cb_ps_outline.data.outlineColor = outlineColor;
	if ( !cb_ps_outline.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 6u, 1u, cb_ps_outline.GetAddressOf() );
	GetStencil( gfx, "Mask" )->Bind( gfx );
	object.Draw();

	// rescale the model and draw using the appropriate shaders and textures
	Shaders::BindShaders( GetContext( gfx ), gfx.vertexShader_light, gfx.pixelShader_light );
	if ( !cb_ps_point.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
	GetStencil( gfx, "Off" )->Bind( gfx );
	object.Draw();
}

void StencilOutline::SpawnControlWindow()
{
	if ( ImGui::Begin( "Stencil Outline Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::ColorEdit3( "Color", &color.x );
		ImGui::SliderFloat( "Scale", &scale, 0.0f, 1.0f, "%.1f" );
	}
	ImGui::End();
}