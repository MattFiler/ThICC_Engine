#include "pch.h"
#include "VBSnail.h"
#include "vertex.h"
#include <iostream>
#include <fstream>
#include "GameStateData.h"
#include "RenderData.h"
#include <d3dcompiler.h>

using namespace std;

VBSnail::VBSnail(RenderData * _RD, std::string _filename, int _sections, float _scale, float _rot, float _step, Color _col1, Color _col2):VBGO3D(_RD)
{
	int width = 0;

	//load line
	ifstream lineFile;
	string fullpath = "../Models/" + _filename + ".txt";
	lineFile.open(fullpath);
	lineFile >> width;

	Vector2* baseLine = new Vector2[width];

	for (int i = 0; i < width; i++)
	{
		lineFile >> baseLine[i].x >> baseLine[i].y;
	}
	
	int numTris = 2 * (width - 1) * (_sections - 1);
	int numVerts = m_numIndices = 3 * numTris;
	myVertex* vertices = new myVertex[numVerts];
	DWORD* indices = new DWORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		vertices[i].texCoord = Vector2::One;
	}	
	
	//build base transform
	Matrix rotTrans = Matrix::CreateRotationY(_rot);
	Matrix stepTrans = Matrix::CreateTranslation(0.0f, _step, 0.0f);
	Matrix scaleTrans = Matrix::CreateScale(_scale);
	Matrix baseTrans = scaleTrans *rotTrans * stepTrans;

	//build array of transforms to rotate,scale and translate baseline around the shell
	Matrix* transforms = new Matrix[_sections];
	transforms[0] = Matrix::Identity;
	for (int i = 1; i < _sections; i++)
	{
		transforms[i] = transforms[i - 1] * baseTrans;
	}

	//build the vertices using these transforms and the line
	//two trinagles for each point scale, turn ad step up
	int vert = 0;
	Vector3 calcPos;
	//transform up the steps
	for (int i = 0; i < (_sections - 1) ; i++)
	{
		for (int j = 0 ; j < (width -1); j++)
		{
			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].Pos = Vector3((float)i, (float)j, 0.0f);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i]);

			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i+1]);
			//vertices[vert++].Pos = Vector3((float)(i + 1), (float)j, 0.0f);

			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].Pos = Vector3((float)i, (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i]);
			
			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			//vertices[vert++].Pos = Vector3((float)(i + 1), (float)j, 0.0f);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i+1]);

			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			//vertices[vert++].Pos = Vector3((float)(i + 1), (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i + 1]);

			vertices[vert].Color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].Pos = Vector3((float)i, (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].Pos = Vector3::Transform(calcPos, transforms[i]);

		}
	}

	//calculate the normals for the basic lighting in the base shader
	CalcNorms(vertices, indices, numVerts);

	BuildIB(_RD, m_numIndices, indices);
	BuildVB(_RD, numVerts, vertices);

	delete[] vertices; //these are all no longer needed as this is now in the Vertex Buffer
	delete[] transforms;
	delete[] indices;
	delete[] baseLine;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = s_psoDesc;
	// compile pixel shader
	ID3DBlob* pixelShader;
	ID3DBlob* errorBuff;
	HRESULT hr = D3DCompileFromFile(L"../shaders/PixelShader2sided.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShader,
		&errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
	}

	// fill out shader bytecode structure for pixel shader
	D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
	psoDesc.PS = pixelShaderBytecode;

	D3D12_RASTERIZER_DESC RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	RS.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState = RS; // a default(ish) rasterizer state.

	// create the pso
	hr = _RD->m_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineStateObject));
}



void VBSnail::Tick(GameStateData* _GSD)
{
	//m_pitch += _GD->m_dt;
	//m_yaw -= _GD->m_dt;
	//m_roll += 0.5f * _GD->m_dt;
	VBGO3D::Tick(_GSD);
}