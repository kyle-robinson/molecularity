#include "PointLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

// "Disco Ball" (https://skfb.ly/6C9ET) by mozillareality is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool PointLight::Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix )
{
	try
	{
		HRESULT hr = cb_ps_point.Initialize( GetDevice( gfx ), GetContext( gfx ) );
		COM_ERROR_IF_FAILED( hr, "Failed to create 'PointLight' constant buffer!" );

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

void PointLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Point Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
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
		if ( ImGui::CollapsingHeader( "Ambient Components" ) )
		{
			ImGui::ColorEdit3( "Colour##1", &ambientColor.x );
			ImGui::SliderFloat( "Intensity##1", &ambientStrength, 0.1f, 1.0f, "%.1f" );
		}
		if ( ImGui::CollapsingHeader( "Diffuse Components" ) )
		{
			ImGui::ColorEdit3( "Colour##2", &diffuseColor.x );
			ImGui::SliderFloat( "Intensity##2", &diffuseStrength, 0.1f, 1.0f, "%.1f" );
		}
		if ( ImGui::CollapsingHeader( "Specular Components" ) )
		{
			ImGui::ColorEdit3( "Colour##3", &specularColor.x );
			ImGui::SliderFloat( "Intensity##3", &specularStrength, 0.1f, 1.0f, "%.1f" );
			ImGui::SliderFloat( "Power", &specularPower, 1.0f, 20.0f, "%1.f" );
		}
		if ( ImGui::CollapsingHeader( "Attenuation" ) )
		{
			ImGui::SliderFloat( "Constant", &constant, 0.05f, 10.0f, "%.2f", 4 );
			ImGui::SliderFloat( "Linear", &linear, 0.0001f, 4.0f, "%.4f", 8 );
			ImGui::SliderFloat( "Quadratic", &quadratic, 0.0000001f, 1.0f, "%.7f", 10 );
		}
	}
	ImGui::End();
}

void PointLight::UpdateConstantBuffer( Graphics& gfx )
{
	cb_ps_point.data.pointAmbientColor = ambientColor;
	cb_ps_point.data.pointAmbientStrength = ambientStrength;
	cb_ps_point.data.pointDiffuseColor = diffuseColor;
	cb_ps_point.data.pointDiffuseStrength = diffuseStrength;
	cb_ps_point.data.pointSpecularColor = specularColor;
	cb_ps_point.data.pointSpecularStrength = specularStrength;
	cb_ps_point.data.pointSpecularPower = specularPower;
	cb_ps_point.data.pointPosition = position;
	cb_ps_point.data.pointConstant = constant;
	cb_ps_point.data.pointLinear = linear;
	cb_ps_point.data.pointQuadratic = quadratic;
	cb_ps_point.data.pointEnable = enable;

	if ( !cb_ps_point.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 3u, 1u, cb_ps_point.GetAddressOf() );
}