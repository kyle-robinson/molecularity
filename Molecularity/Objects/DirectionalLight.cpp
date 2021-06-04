#include "DirectionalLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

// "Disco Ball" (https://skfb.ly/6C9ET) by mozillareality is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool DirectionalLight::Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix )
{
	try
	{
		HRESULT hr = cb_ps_directional.Initialize( GetDevice( gfx ), GetContext( gfx ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'DirectionalLight' constant buffer!" );

		//if ( !Light::Initialize( "Resources\\Models\\LightBall\\scene.gltf", GetDevice( gfx ), GetContext( gfx ), cb_vs_matrix ) )
		//	return false;
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void DirectionalLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Directional Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::Text( "Usage: " );
		ImGui::SameLine();
		static int enableGroup = 0;
		if ( ImGui::RadioButton( "Enable", &enableGroup, 0 ) )
			enable = TRUE;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Disable", &enableGroup, 1 ) )
			enable = FALSE;

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

void DirectionalLight::UpdateConstantBuffer( Graphics& gfx )
{
	cb_ps_directional.data.directionalEnable = enable;
	cb_ps_directional.data.directionalPosition = position;
	cb_ps_directional.data.directionalDiffuseColor = diffuseColor;
	cb_ps_directional.data.directionalDiffuseStrength = diffuseStrength;
	cb_ps_directional.data.directionalSpecularColor = specularColor;
	cb_ps_directional.data.directionalSpecularStrength = specularStrength;
	cb_ps_directional.data.directionalSpecularPower = specularPower;

	if ( !cb_ps_directional.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 4u, 1u, cb_ps_directional.GetAddressOf() );
}