#include "DirectionalLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

// "Disco Ball" (https://skfb.ly/6C9ET) by mozillareality is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

void DirectionalLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Directional Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::SliderFloat3( "Position", &position.x, -20.0f, 20.0f, "%.1f" );
		if ( ImGui::CollapsingHeader( "Diffuse Components" ) )
		{
			ImGui::ColorEdit3( "Colour##1", &diffuseColor.x );
			ImGui::SliderFloat( "Intensity##1", &diffuseStrength, 0.1f, 1.0f, "%.1f" );
		}
		if ( ImGui::CollapsingHeader( "Specular Components" ) )
		{
			ImGui::ColorEdit3( "Colour##2", &specularColor.x );
			ImGui::SliderFloat( "Intensity##2", &specularStrength, 0.1f, 1.0f, "%.1f" );
			ImGui::SliderFloat( "Power", &specularPower, 1.0f, 20.0f, "%1.f" );
		}
	}
	ImGui::End();
}

void DirectionalLight::UpdateConstantBuffer( ConstantBuffer<CB_PS_directional>& cb_ps_directional )
{
	cb_ps_directional.data.directionalPosition = position;
	cb_ps_directional.data.directionalDiffuseColor = diffuseColor;
	cb_ps_directional.data.directionalDiffuseStrength = diffuseStrength;
	cb_ps_directional.data.directionalSpecularColor = specularColor;
	cb_ps_directional.data.directionalSpecularStrength = specularStrength;
	cb_ps_directional.data.directionalSpecularPower = specularPower;
}