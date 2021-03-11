#pragma once
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <dxtk/PostProcess.h>
#include "GraphicsResource.h"

class PostProcessing : public GraphicsResource
{
public:
	PostProcessing( GraphicsContainer& gfx );
	void Bind( GraphicsContainer& gfx ) noexcept override;
	void SpawnControlWindow();
private:
	// post-processing parameters
	float bloomThreshold;
	bool bloomBlurHorizontal;
	float bloomBlurSize;
	float bloomBlurBrightness;
	float gaussianMultiplier;

	// post-processing options
	bool useBasicPostProcess;
	BasicPostProcess::Effect basicEffect;
	ToneMapPostProcess::Operator toneMapOperator;
	ToneMapPostProcess::TransferFunction toneMapTransferFunction;

	// post-processing components
	std::shared_ptr<BasicPostProcess> postProcessBasic;
	std::shared_ptr<ToneMapPostProcess> postProcessToneMap;
};

#endif