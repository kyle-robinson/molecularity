#pragma once
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <dxtk/PostProcess.h>
#include "GraphicsResource.h"

class PostProcessing : public GraphicsResource
{
public:
	PostProcessing( Graphics& gfx );
	void Bind( Graphics& gfx ) noexcept override;
	void SpawnControlWindow();
	
	void BindMonochrome() noexcept { basicEffect = BasicPostProcess::Effect::Monochrome; }
	void BindSepia() noexcept { basicEffect = BasicPostProcess::Effect::Sepia; }
	void UnbindEffect() noexcept { basicEffect = BasicPostProcess::Effect::Copy; }
private:
	// Post-processing parameters
	float bloomThreshold;
	bool bloomBlurHorizontal;
	float bloomBlurSize;
	float bloomBlurBrightness;
	float gaussianMultiplier;

	// Post-processing options
	bool useBasicPostProcess;
	BasicPostProcess::Effect basicEffect;
	ToneMapPostProcess::Operator toneMapOperator;
	ToneMapPostProcess::TransferFunction toneMapTransferFunction;

	// Post-processing components
	std::shared_ptr<BasicPostProcess> postProcessBasic;
	std::shared_ptr<ToneMapPostProcess> postProcessToneMap;
};

#endif