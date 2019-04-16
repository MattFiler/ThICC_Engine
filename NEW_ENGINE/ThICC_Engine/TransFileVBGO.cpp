#include "pch.h"
#include "TransFileVBGO.h"
#include "RenderData.h"


TransFileVBGO::TransFileVBGO(std::string _fileName, RenderData * _RD):FileVBGO(_fileName,_RD)
{
	//sets up the PSO witha render state to draw this file transparent
	//obviously we would then need to do a proper ordered draw
	//better still seperate opaque and transparentr render passes

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = s_psoDesc;
	psoDesc.BlendState = CommonStates::AlphaBlend; // an alpha blend state.
	psoDesc.DepthStencilState = CommonStates::DepthRead; //only read not write to the depth buffer
	// create the pso
	HRESULT hr = _RD->m_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineStateObject));

}


TransFileVBGO::~TransFileVBGO()
{
}
