#include "stdafx.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "PostProcessing.h"
#include <imgui/imgui.h>

PostProcessing::PostProcessing( GraphicsContainer& gfx )
{
	postProcessBasic = std::make_shared<BasicPostProcess>( GetDevice( gfx ) );
	postProcessToneMap = std::make_shared<ToneMapPostProcess>( GetDevice( gfx ) );

	useBasicPostProcess = true;
	basicEffect = BasicPostProcess::Effect::Copy;
	toneMapOperator = ToneMapPostProcess::Operator::None;
	toneMapTransferFunction = ToneMapPostProcess::TransferFunction::Linear;
}

void PostProcessing::Bind( GraphicsContainer& gfx ) noexcept
{
	// BASIC POST-PROCESS
	if ( useBasicPostProcess )
	{
		postProcessBasic->SetEffect( basicEffect );

		// set additional effect if using bloom effect
		if ( basicEffect == BasicPostProcess::Effect::BloomExtract )
			postProcessBasic->SetEffect( BasicPostProcess::BloomBlur );

		postProcessBasic->SetSourceTexture( gfx.GetRenderTarget()->GetShaderResourceView() );
		postProcessBasic->Process( GetContext( gfx ) );
	}
	// TONE MAP POST-PROCESS
	else
	{
		postProcessToneMap->SetOperator( toneMapOperator );
		postProcessToneMap->SetTransferFunction( toneMapTransferFunction );
		postProcessToneMap->SetHDRSourceTexture( gfx.GetRenderTarget()->GetShaderResourceView() );
		postProcessToneMap->Process( GetContext( gfx ) );
	}
}

void PostProcessing::SpawnControlWindow()
{
	if ( ImGui::Begin( "Post-Processing", FALSE, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ) )
	{
		// switch between basic/tone map post-process options
		ImGui::Text( "Post-Process Type:" );
		ImGui::SameLine();
		static int postProcessGroup = 0;
		if ( ImGui::RadioButton( "Basic", &postProcessGroup, 0 ) )
			useBasicPostProcess = true;
		ImGui::SameLine();
		if ( ImGui::RadioButton( "Tone Map", &postProcessGroup, 1 ) )
			useBasicPostProcess = false;

		if ( useBasicPostProcess )
		{
			// overlay effects
			static int activeEffect = 0;
			static bool selectedEffect[6];
			static std::string previewValueEffect = "None";
			static const char* effectList[]{ "None", "Sepia", "Monochrome", "Gaussian Blur", "Down-Scale", "Bloom" };
			if ( ImGui::BeginCombo( "Overlay Effect", previewValueEffect.c_str() ) )
			{
				for ( unsigned int i = 0; i < IM_ARRAYSIZE( effectList ); i++ )
				{
					const bool isSelected = i == activeEffect;
					if ( ImGui::Selectable( effectList[i], isSelected ) )
					{
						activeEffect = i;
						previewValueEffect = effectList[i];
					}
				}

				switch ( activeEffect )
				{
				case 0: basicEffect = BasicPostProcess::Effect::Copy; break;
				case 1: basicEffect = BasicPostProcess::Effect::Sepia; break;
				case 2: basicEffect = BasicPostProcess::Effect::Monochrome; break;
				case 3: basicEffect = BasicPostProcess::Effect::GaussianBlur_5x5; break;
				case 4: basicEffect = BasicPostProcess::Effect::DownScale_4x4; break;
				case 5: basicEffect = BasicPostProcess::Effect::BloomExtract; break;
				}

				ImGui::EndCombo();
			}
		}
		else
		{
			// tone map operator
			static int activeOperator = 0;
			static bool selectedOperator[4];
			static std::string previewValueOperator = "None";
			static const char* operatorList[]{ "None", "Saturate", "Reinhard", "ACESFilmic" };
			if ( ImGui::BeginCombo( "Operator", previewValueOperator.c_str() ) )
			{
				for ( unsigned int i = 0; i < IM_ARRAYSIZE( operatorList ); i++ )
				{
					const bool isSelected = i == activeOperator;
					if ( ImGui::Selectable( operatorList[i], isSelected ) )
					{
						activeOperator = i;
						previewValueOperator = operatorList[i];
					}
				}

				switch ( activeOperator )
				{
				case 0: toneMapOperator = ToneMapPostProcess::Operator::None; break;
				case 1: toneMapOperator = ToneMapPostProcess::Operator::Saturate; break;
				case 2: toneMapOperator = ToneMapPostProcess::Operator::Reinhard; break;
				case 3: toneMapOperator = ToneMapPostProcess::Operator::ACESFilmic; break;
				}

				ImGui::EndCombo();
			}

			// tone map transfer function
			static int activeTransfer = 0;
			static bool selectedTransfer[3];
			static std::string previewValueTransfer = "Linear";
			static const char* transferList[]{ "Linear", "SRGB", "ST2084" };
			if ( ImGui::BeginCombo( "Transfer Function", previewValueTransfer.c_str() ) )
			{
				for ( unsigned int i = 0; i < IM_ARRAYSIZE( transferList ); i++ )
				{
					const bool isSelected = i == activeTransfer;
					if ( ImGui::Selectable( transferList[i], isSelected ) )
					{
						activeTransfer = i;
						previewValueTransfer = transferList[i];
					}
				}

				switch ( activeTransfer )
				{
				case 0: toneMapTransferFunction = ToneMapPostProcess::TransferFunction::Linear; break;
				case 1: toneMapTransferFunction = ToneMapPostProcess::TransferFunction::SRGB; break;
				case 2: toneMapTransferFunction = ToneMapPostProcess::TransferFunction::ST2084; break;
				}

				ImGui::EndCombo();
			}
		}	
	}
	ImGui::End();
}