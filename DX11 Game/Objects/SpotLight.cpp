#include "SpotLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

void SpotLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Spot Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		ImGui::SliderFloat3( "Position", &spotLightPosition.x, -20.0f, 20.0f, "%.1f" );
		ImGui::ColorEdit3( "Colour", &spotLightColor.x );
		ImGui::SliderFloat( "Inner Cutoff", &innerCutoff, 0.0f, 1.0f, "%.1f" );
	}
	ImGui::End();
}

void SpotLight::UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot )
{
	cb_ps_spot.data.innerCutoff = innerCutoff;
	cb_ps_spot.data.outerCutoff = outerCutoff;
	cb_ps_spot.data.spotColor = spotLightColor;
	cb_ps_spot.data.spotPosition = spotLightPosition;
	cb_ps_spot.data.spotDirection = spotLightDirection;
}