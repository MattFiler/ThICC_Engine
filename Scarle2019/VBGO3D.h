#pragma once
#include "GameObject3D.h"
#include "ConstantBuffer.h"
#include "GameFilepaths.h"
//A basic DirectX12 renderer for drawing from a Vertex and Index Buffer
//all things DX12 stuff required have two versions
// s_  static version shared by all VBGO3D objects
// m_  member versions own by this object
// if the m_ version is NULL then the static version will be used
// based on these tutorials: https://www.braynzarsoft.net/viewtutorial/q16390-04-directx-12-braynzar-soft-tutorials
// You'll also find there a long explanation of what is going on with all the setting up of stuff for DirectX 12

// this will only call release if an object exists (prevents exceptions calling release on non existant objects)
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

struct myVertex;
class VBGO3D :
	public GameObject3D
{
public:
	VBGO3D();
	virtual ~VBGO3D();

	virtual void Tick() override;
	virtual void Render() override;

	//set up (and clean up) all static objects to be shared between these objects
	static bool SetUpVBGOs();
	static void CleanUp();

	virtual void UpdateConstBuff();

	//push all IBs & VBs up to GPU
	static void PushIBVB();

	void CalcNorms(myVertex* _verts, DWORD* _indices, int _numI);

protected:

	//static shared versions
	static ID3D12PipelineState* s_pipelineStateObject; // pso containing a pipeline state
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC s_psoDesc; //description to create the default pipeline state
	static ID3D12RootSignature* s_rootSignature; // root signature defines data shaders will access

	//constant buffer stuff
	static const UINT		c_swapBufferCount = 2; //make sure this matches the one in Game.h
	ID3D12DescriptorHeap* m_mainDescriptorHeap[c_swapBufferCount]; // this heap will store the descriptor to our constant buffer
	ID3D12Resource* m_constantBufferUploadHeap[c_swapBufferCount]; // this is the memory on the gpu where our constant buffer will be placed.

	ConstantBuffer m_ConstBuff; // this is the constant buffer data we will send to the gpu 

	UINT8* m_ConstBuffGPUAddress[c_swapBufferCount]; // this is a pointer to the memory location we get when we map our constant buffer


	//this objects versions
	ID3D12PipelineState* m_pipelineStateObject = NULL; 
	ID3D12RootSignature* m_rootSignature = NULL;  

	//index & Vertex Buffer stuff
	ID3D12Resource* m_vertexBuffer; 
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	ID3D12Resource* m_indexBuffer; 
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	int m_numIndices = 0;

	//once populated build an Index Buffer
	void BuildIB(int _numI, void* _indices);
	//once populated build a Vertex Buffer
	void BuildVB(int _numVerts, void* _vertices);

	//textures
	static ID3D12Resource* s_texture;
	ID3D12Resource* m_texture = NULL;

	GameFilepaths m_filepath;
};

