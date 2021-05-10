#include "SpotLight.h"
#include "Camera.h"
#include <imgui/imgui.h>

// "Flashlight" (https://skfb.ly/6QXJG) by Brandon Baldwin is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
// FREE Low Poly Cyberpunk-Sci fi Handgun Free low-poly 3D model (https://www.cgtrader.com/free-3d-models/military/gun/free-low-poly-cyberpunk-sci-fi-handgun)

bool SpotLight::Initialize( Graphics& gfx, ConstantBuffer<CB_VS_matrix>& cb_vs_matrix )
{
	try
	{
		HRESULT hr = cb_ps_spot.Initialize( GetDevice( gfx ), GetContext( gfx ) );
		COM_ERROR_IF_FAILED( hr, "Failed to initialize 'SpotLight' constant buffer!" );

		//if ( !Light::Initialize( "Resources\\Models\\Flashlight.fbx", GetDevice( gfx ), GetContext( gfx ), cb_vs_matrix ) )
		//if ( !Light::Initialize( "Resources\\Models\\ShrinkGun3.fbx", GetDevice( gfx ), GetContext( gfx ), cb_vs_matrix ) )
		if ( !Light::Initialize( "Resources\\Models\\Cybergun.fbx", GetDevice( gfx ), GetContext( gfx ), cb_vs_matrix ) )
			return false;
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}
	return true;
}

void SpotLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Spot Light", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		ImGui::Text( "Usage: " );
		ImGui::SameLine();
		static int enableGroup = 0;
		if ( ImGui::RadioButton( "Enable", &enableGroup, 0 ) )
			enable = TRUE;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Disable", &enableGroup, 1 ) )
			enable = FALSE;

		ImGui::ColorEdit3( "Colour", &color.x );
		ImGui::SliderFloat( "Cone", &cone, 5.0f, 40.0f, "%1.f" );
		ImGui::SliderFloat( "Range", &range, 500.0f, 10000.0f, "%10.f" );
		ImGui::SliderFloat( "Intensity", &strength, 0.1f, 2.0f, "%.1f" );
	}
	ImGui::End();
}

void SpotLight::UpdateConstantBuffer( Graphics& gfx, std::unique_ptr<Camera>& camera )
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

	if ( !cb_ps_spot.ApplyChanges() ) return;
	GetContext( gfx )->PSSetConstantBuffers( 5u, 1u, cb_ps_spot.GetAddressOf() );
}

void SpotLight::UpdateModelPosition( std::unique_ptr<Camera>& camera )
{
	XMVECTOR spotLightPosition = camera->GetPositionVector();
	spotLightPosition += camera->GetForwardVector() * 4;
	spotLightPosition += camera->GetRightVector() * 2;
	SetPosition(
		XMVectorGetX( spotLightPosition ),
		XMVectorGetY( spotLightPosition ) - 1.0f,
		XMVectorGetZ( spotLightPosition )
	);
	SetRotation(
		camera->GetRotationFloat3().z,
		camera->GetRotationFloat3().y - XM_PIDIV2,
		-camera->GetRotationFloat3().x
	);
}