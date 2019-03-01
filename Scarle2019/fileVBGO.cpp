#include "pch.h"
#include <iostream>
#include <fstream>
#include "fileVBGO.h"
#include "RenderData.h"
//#include "helper.h"
#include "DDSTextureLoader.h"
#include "vertex.h"
#include <DirectXColors.h>

using namespace DirectX;
using namespace std;

FileVBGO::FileVBGO(string _fileName, RenderData * _RD):VBGO3D(_RD)
{
	//open file
	ifstream meshFile;
	string fullpath = m_filepath.generateFilepath(_fileName, m_filepath.MODEL_CUSTOM);
	meshFile.open(fullpath);

	//what data does this file give
	m_texCoords = false;
	m_colour = false;

	char texOrColour;
	meshFile >> texOrColour;

	if (texOrColour == 'T')
	{
		m_texCoords = true;
		//get texture fileName
		string  texFileName;
		meshFile >> texFileName;

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		string fullpath = m_filepath.generateFilepath(texFileName, m_filepath.IMAGE);
		std::wstring wFilename = converter.from_bytes(fullpath.c_str());

		ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

		resourceUpload.Begin();

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(_RD->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),&m_texture));

		auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

		uploadResourcesFinished.wait();

		for (int i = 0; i < c_swapBufferCount; ++i)
		{
			CreateShaderResourceView(_RD->m_d3dDevice.Get(), m_texture, m_mainDescriptorHeap[i]->GetCPUDescriptorHandleForHeapStart());
		}
	}
	else
	{		
		m_colour = true;
	}

	//get number of vertices
	int numVerts = 0;
	meshFile >> numVerts;
	myVertex* vertices = new myVertex[numVerts];

	//load verts
	int dummy;
	for (int i = 0; i< numVerts; i++)
	{
		meshFile >> dummy >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;

		if (m_texCoords)
		{
			meshFile >> vertices[i].texCoord.x >> vertices[i].texCoord.y;
			vertices[i].Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		}

		if (m_colour)
		{
			float R, G, B, A;
			meshFile >> R >> G >> B >> A;
			vertices[i].Color = Color(R, G, B, A);
			vertices[i].texCoord = Vector2::Zero;
		}
	}

	//load indices
	int numPrims = 0;
	meshFile >> numPrims;
	m_numIndices = 3 * numPrims;
	DWORD* indices = new DWORD[m_numIndices];
	for (unsigned int i = 0; i < numPrims; i++)
	{
		WORD V1, V2, V3;
		meshFile >> dummy >> V1 >> V2 >> V3;

		indices[3 * i] = V1;
		indices[3 * i + 1] = V2;
		indices[3 * i + 2] = V3;
	}
	CalcNorms(vertices, indices, m_numIndices);

	BuildIB(_RD, m_numIndices, indices);
	BuildVB(_RD, numVerts, vertices);

	//tidy up
	meshFile.close();
	delete[] vertices;
	delete[] indices;
}

FileVBGO::~FileVBGO()
{

}