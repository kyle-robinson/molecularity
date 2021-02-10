#include "PointLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

void PointLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Point Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		if ( ImGui::CollapsingHeader( "Ambient Components" ) )
		{
			ImGui::ColorEdit3( "Colour##1", &ambientColor.x );
			ImGui::SliderFloat( "Intensity##1", &ambientStrength, 0.1f, 1.0f, "%.1f" );
		}
		if ( ImGui::CollapsingHeader( "Diffuse Components" ) )
		{
			ImGui::ColorEdit3( "Colour##2", &lightColor.x );
			ImGui::SliderFloat( "Intensity##2", &lightStrength, 0.1f, 1.0f, "%.1f" );
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

void PointLight::UpdateConstantBuffer( ConstantBuffer<CB_PS_point>& cb_ps_point, std::unique_ptr<Camera>& camera )
{
	cb_ps_point.data.pointAmbientColor = ambientColor;
	cb_ps_point.data.pointAmbientStrength = ambientStrength;
	cb_ps_point.data.pointDiffuseColor = lightColor;
	cb_ps_point.data.pointDiffuseStrength = lightStrength;
	cb_ps_point.data.pointSpecularColor = specularColor;
	cb_ps_point.data.pointSpecularStrength = specularStrength;
	cb_ps_point.data.pointSpecularPower = specularPower;

	//XMVECTOR lightPosition = camera->GetPositionVector();
	//lightPosition += camera->GetForwardVector();
	//lightPosition += camera->GetRightVector() / 4;
	//XMFLOAT3 lightPositionF = XMFLOAT3( XMVectorGetX( lightPosition ), XMVectorGetY( lightPosition ), XMVectorGetZ( lightPosition ) );
	cb_ps_point.data.pointPosition = lightPosition;
	
	cb_ps_point.data.lightConstant = constant;
	cb_ps_point.data.lightLinear = linear;
	cb_ps_point.data.lightQuadratic = quadratic;
}