#include "Fog.h"
#include <imgui/imgui.h>

Fog::Fog( GraphicsContainer& gfx ) : color( XMFLOAT3( 0.3f, 0.1f, 0.025f ) ), start( 25.0f ), end( 75.0f )
{
	try
	{
		HRESULT hr = cb_vs_fog.Initialize( GetDevice( gfx ), GetContext( gfx ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'fog' constant buffer!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return;
	}
}

void Fog::UpdateConstantBuffer( GraphicsContainer& gfx ) noexcept
{
	cb_vs_fog.data.fogEnable = enable;
	cb_vs_fog.data.fogColor = color;
	cb_vs_fog.data.fogStart = start;
	cb_vs_fog.data.fogEnd = end;

	if ( !cb_vs_fog.ApplyChanges() ) return;
	GetContext( gfx )->VSSetConstantBuffers( 1u, 1u, cb_vs_fog.GetAddressOf() );
	GetContext( gfx )->PSSetConstantBuffers( 1u, 1u, cb_vs_fog.GetAddressOf() );
}

void Fog::SpawnControlWindow()
{
	if ( ImGui::Begin( "Fog Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::Text( "Usage: " );
		ImGui::SameLine();
		static int enableGroup = 0;
		if ( ImGui::RadioButton( "Enable", &enableGroup, 0 ) )
			enable = TRUE;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Disable", &enableGroup, 1 ) )
			enable = FALSE;

		ImGui::ColorEdit3( "Fog Colour", &color.x );
		ImGui::SliderFloat( "Start Distance", &start, 1.0f, end - 0.1f );
		ImGui::SliderFloat( "End Distance", &end, start + 0.1f, 100.0f  );
	}
	ImGui::End();
}