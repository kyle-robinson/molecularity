#include "SpotLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

// "Flashlight" (https://skfb.ly/6QXJG) by Brandon Baldwin is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

void SpotLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Spot Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::Text( "Usage: " );
		ImGui::SameLine();
		static int enableGroup = 0;
		if ( ImGui::RadioButton( "Enable", &enableGroup, 0 ) )
			enable = 1.0f;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Disable", &enableGroup, 1 ) )
			enable = 0.0f;

		ImGui::ColorEdit3( "Colour", &color.x );
		ImGui::SliderFloat( "Cone", &cone, 5.0f, 40.0f, "%1.f" );
		ImGui::SliderFloat( "Range", &range, 500.0f, 10000.0f, "%10.f" );
		ImGui::SliderFloat( "Intensity", &strength, 0.1f, 1.0f, "%.1f" );
	}
	ImGui::End();
}

void SpotLight::UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot, std::unique_ptr<Camera>& camera )
{
	cb_ps_spot.data.spotEnable = enable;
	cb_ps_spot.data.spotCone = cone;
	cb_ps_spot.data.spotRange = range;
	cb_ps_spot.data.spotDiffuseColor = color;
	cb_ps_spot.data.spotDiffuseStrength = strength;
	
	XMVECTOR lightPosition = camera->GetPositionVector();
	lightPosition += camera->GetForwardVector();
	lightPosition += camera->GetRightVector() / 4;
	XMFLOAT3 lightPositionF = XMFLOAT3( XMVectorGetX( lightPosition ), XMVectorGetY( lightPosition ), XMVectorGetZ( lightPosition ) );
	cb_ps_spot.data.spotPosition = camera->GetPositionFloat3();
	
	direction.x = camera->GetCameraTarget().x - camera->GetPositionFloat3().x;
	direction.y = camera->GetCameraTarget().y - camera->GetPositionFloat3().y;
	direction.z = camera->GetCameraTarget().z - camera->GetPositionFloat3().z;
	cb_ps_spot.data.spotDirection = direction;
}