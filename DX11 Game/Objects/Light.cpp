#include "Light.h"
//#include "Camera.h"
//#include "PointLight.h"
//#include "DirectionalLight.h"
//#include <imgui/imgui.h>

//"Flashlight" (https://skfb.ly/6QXJG) by Brandon Baldwin is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).

bool Light::Initialize( ID3D11Device* device, ID3D11DeviceContext* context,
	ConstantBuffer<CB_VS_matrix>& cb_vs_vertexshader )
{
	if ( !model.Initialize( "Resources\\Models\\Flashlight.fbx", device, context, cb_vs_vertexshader ) )
		return false;
	SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	UpdateMatrix();
	return true;
}

/*void Light::SpawnControlWindow()
{
	if ( ImGui::Begin( "Light Controls", FALSE, ImGuiWindowFlags_AlwaysAutoResize ) )
	{
		DirectionalLight directionalLight;
		directionalLight.SpawnControlWindow();

		PointLight pointLight;
		pointLight.SpawnControlWindow();

		if ( ImGui::CollapsingHeader( "Directional Light" ) )
		{
			ImGui::SliderFloat3( "Position", &directionalLightPosition.x, -20.0f, 20.0f, "%.1f" );
			ImGui::ColorEdit3( "Colour", &directionalLightColor.x );
			ImGui::SliderFloat( "Intensity##0", &directionalLightStrength, 0.1f, 1.0f, "%.1f" );
		}
		if ( ImGui::CollapsingHeader( "Point Light" ) )
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
	} ImGui::End();
}
*/
/*void Light::UpdateConstantBuffer( ConstantBuffer<CB_PS_light>& cb_ps_light, std::unique_ptr<Camera>& camera )
{
	cb_ps_light.data.ambientLightColor = ambientColor;
	cb_ps_light.data.ambientLightStrength = ambientStrength;
	cb_ps_light.data.dynamicLightColor = lightColor;
	cb_ps_light.data.dynamicLightStrength = lightStrength;
	cb_ps_light.data.specularLightColor = specularColor;
	cb_ps_light.data.specularLightStrength = specularStrength;
	cb_ps_light.data.specularLightPower = specularPower;

	XMVECTOR lightPosition = camera->GetPositionVector();
	lightPosition += camera->GetForwardVector();
	lightPosition += camera->GetRightVector() / 4;
	XMFLOAT3 lightPositionF = XMFLOAT3( XMVectorGetX( lightPosition ), XMVectorGetY( lightPosition ), XMVectorGetZ( lightPosition ) );
	cb_ps_light.data.dynamicLightPosition = lightPositionF;
	
	cb_ps_light.data.lightConstant = constant;
	cb_ps_light.data.lightLinear = linear;
	cb_ps_light.data.lightQuadratic = quadratic;

	cb_ps_light.data.directionalLightPosition = directionalLightPosition;
	cb_ps_light.data.directionalLightColor = directionalLightColor;
	cb_ps_light.data.directionalLightStrength = directionalLightStrength;
}*/