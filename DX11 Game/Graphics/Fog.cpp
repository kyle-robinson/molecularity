#include "stdafx.h"
#include "Fog.h"
#include "Graphics.h"
#include <imgui/imgui.h>

Fog::Fog() : color( XMFLOAT3( 0.3f, 0.1f, 0.025f ) ), start( 25.0f ), end( 75.0f ) { }

Fog::Fog( XMFLOAT3 fogColor, float fogStart, float fogEnd )
	: color( fogColor ), start( fogStart ), end( fogEnd ) { }

void Fog::UpdateConstantBuffer( ConstantBuffer<CB_VS_fog>& cb_vs_fog ) noexcept
{
	cb_vs_fog.data.fogEnable = enable;
	cb_vs_fog.data.fogColor = color;
	cb_vs_fog.data.fogStart = start;
	cb_vs_fog.data.fogEnd = end;
}

void Fog::SpawnControlWindow()
{
	if ( ImGui::Begin( "Fog Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
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