#include "SpotLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

void SpotLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Spot Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		//ImGui::SliderFloat3( "Position", &spotLightPosition.x, -20.0f, 20.0f, "%.1f" );
		//ImGui::SliderFloat3( "Direction", &spotLightDirection.x, 0.0f, 1.0f, "%.1f" );
		ImGui::ColorEdit3( "Colour", &spotLightColor.x );
		//ImGui::SliderFloat( "Inner Cutoff", &innerCutoff, 0.0f, 1.0f, "%.1f" );
		//ImGui::SliderFloat( "Outer Cutoff", &outerCutoff, 1.0f, 50.0f, "%.1f" );

		ImGui::SliderFloat( "Cone", &cone, 5.0f, 40.0f, "%1.f" );
		ImGui::SliderFloat( "Range", &range, 500.0f, 10000.0f, "%10.f" );
	}
	ImGui::End();
}

void SpotLight::UpdateConstantBuffer( ConstantBuffer<CB_PS_spot>& cb_ps_spot, std::unique_ptr<Camera>& camera )
{
	//cb_ps_spot.data.innerCutoff = innerCutoff;
	//cb_ps_spot.data.outerCutoff = outerCutoff;
	cb_ps_spot.data.range = range;
	cb_ps_spot.data.cone = cone;

	//cb_ps_spot.data.spotColor = spotLightColor;
	cb_ps_spot.data.spotAmbient = { 0.0f, 0.0f, 0.0f };
	cb_ps_spot.data.spotDiffuse = { 1.0f, 1.0f, 1.0f };
	
	//XMVECTOR lightPosition = camera->GetPositionVector();
	//lightPosition += camera->GetForwardVector();
	//lightPosition += camera->GetRightVector() / 4;
	//XMFLOAT3 lightPositionF = XMFLOAT3( XMVectorGetX( lightPosition ), XMVectorGetY( lightPosition ), XMVectorGetZ( lightPosition ) );
	cb_ps_spot.data.spotPosition = camera->GetPositionFloat3();
	
	spotLightDirection.x = camera->GetCameraTarget().x - camera->GetPositionFloat3().x;
	spotLightDirection.y = camera->GetCameraTarget().y - camera->GetPositionFloat3().y;
	spotLightDirection.z = camera->GetCameraTarget().z - camera->GetPositionFloat3().z;
	cb_ps_spot.data.spotDirection = spotLightDirection;
}