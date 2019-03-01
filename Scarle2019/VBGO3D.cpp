#include "pch.h"
#include <d3dcompiler.h>
#include "vertex.h"
#include "VBGO3D.h"
#include "RenderData.h"

#pragma comment(lib, "d3dcompiler.lib")

//set up static members
ID3D12PipelineState* VBGO3D::s_pipelineStateObject = NULL;
D3D12_GRAPHICS_PIPELINE_STATE_DESC VBGO3D::s_psoDesc = {};
ID3D12RootSignature* VBGO3D::s_rootSignature = NULL;
ID3D12Resource* VBGO3D::s_texture= NULL;

VBGO3D::VBGO3D(RenderData * _RD)
{

	// create the constant buffer resource heap
	// We will update the constant buffer one or more times per frame, so we will use only an upload heap
	// unlike previously we used an upload heap to upload the vertex and index data, and then copied over
	// to a default heap. If you plan to use a resource for more than a couple frames, it is usually more
	// efficient to copy to a default heap where it stays on the gpu. In this case, our constant buffer
	// will be modified and uploaded at least once per frame, so we only use an upload heap
	HRESULT hr;

	for (int i = 0; i < c_swapBufferCount; ++i)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hr = _RD->m_d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_mainDescriptorHeap[i]));
		CreateShaderResourceView(_RD->m_d3dDevice.Get(), s_texture, m_mainDescriptorHeap[i]->GetCPUDescriptorHandleForHeapStart());
	}

	// create a resource heap, descriptor heap, and pointer to cbv for each frame
	for (int i = 0; i < c_swapBufferCount; ++i)
	{
		hr = _RD->m_d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // this heap will be used to upload the constant buffer data
			D3D12_HEAP_FLAG_NONE, // no flags
			&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64), // size of the resource heap. Must be a multiple of 64KB for single-textures and constant buffers
			D3D12_RESOURCE_STATE_GENERIC_READ, // will be data that is read from so we keep it in the generic read state
			nullptr, // we do not have use an optimized clear value for constant buffers
			IID_PPV_ARGS(&m_constantBufferUploadHeap[i]));
		m_constantBufferUploadHeap[i]->SetName(L"Constant Buffer Upload Resource Heap");

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantBufferUploadHeap[i]->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (sizeof(ConstantBuffer) + 255) & ~255;    // CB size is required to be 256-byte aligned.
		//_RD->m_d3dDevice->CreateConstantBufferView(&cbvDesc, m_mainDescriptorHeap[i]->GetCPUDescriptorHandleForHeapStart());

		ZeroMemory(&m_ConstBuff, sizeof(m_ConstBuff));

		CD3DX12_RANGE readRange(0, 0);    // We do not intend to read from this resource on the CPU. (End is less than or equal to begin)
		hr = m_constantBufferUploadHeap[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_ConstBuffGPUAddress[i]));
		memcpy(m_ConstBuffGPUAddress[i], &m_ConstBuff, sizeof(m_ConstBuff));
	}

}

VBGO3D::~VBGO3D()
{
	//TIDY EVERYTHING UP
	SAFE_RELEASE(m_pipelineStateObject);
	SAFE_RELEASE(m_rootSignature);

	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);
	for (int i = 0; i < c_swapBufferCount; ++i)
	{
		SAFE_RELEASE(m_mainDescriptorHeap[i]);
		SAFE_RELEASE(m_constantBufferUploadHeap[i]);
	};

	SAFE_RELEASE(m_texture);
}

void VBGO3D::CleanUp()
{
	SAFE_RELEASE(s_pipelineStateObject);
	SAFE_RELEASE(s_rootSignature);
	SAFE_RELEASE(s_texture);
}

void VBGO3D::Tick(GameStateData * _GSD)
{		
	GameObject3D::Tick(_GSD);
}

void VBGO3D::Render(RenderData * _RD)
{
	ID3D12RootSignature* useRS = m_rootSignature ? m_rootSignature : s_rootSignature;
	_RD->m_commandList->SetGraphicsRootSignature(useRS); // set the root signature

	UpdateConstBuff(_RD);
	
	// draw triangle
	ID3D12PipelineState* usePSO = m_pipelineStateObject ? m_pipelineStateObject : s_pipelineStateObject;
	_RD->m_commandList->SetPipelineState(usePSO);
	_RD->m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology
	_RD->m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView); // set the vertex buffer (using the vertex buffer view)
	_RD->m_commandList->IASetIndexBuffer(&m_indexBufferView);
	// set the descriptor heap
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_mainDescriptorHeap[*_RD->m_backBufferIndex] };
	_RD->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	// set the root descriptor table 0 to the constant buffer descriptor heap
	_RD->m_commandList->SetGraphicsRootDescriptorTable(1, m_mainDescriptorHeap[*_RD->m_backBufferIndex]->GetGPUDescriptorHandleForHeapStart());

	_RD->m_commandList->DrawIndexedInstanced(m_numIndices, 1, 0, 0, 0); // draw all my indices
}

bool VBGO3D::SetUpVBGOs(RenderData * _RD)
{
	// create root signature
	GameFilepaths m_filepath; //WHY?!

	// create a root descriptor, which explains where to find the data for this root parameter
	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.RegisterSpace = 0;
	rootCBVDescriptor.ShaderRegister = 0;

	// create a descriptor range (descriptor table) and fill it out
	// this is a range of descriptors inside a descriptor heap
	D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[1]; // only one range right now
	descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // this is a range of shader resource views (descriptors)
	descriptorTableRanges[0].NumDescriptors = 1; // we only have one texture right now, so the range is only 1
	descriptorTableRanges[0].BaseShaderRegister = 0; // start index of the shader registers in the range
	descriptorTableRanges[0].RegisterSpace = 0; // space 0. can usually be zero
	descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // this appends the range to the end of the root signature descriptor tables

	// create a descriptor table
	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); // we only have one range
	descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; // the pointer to the beginning of our ranges array

	// create a root parameter for the root descriptor and fill it out
	D3D12_ROOT_PARAMETER  rootParameters[2]; // two root parameters
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // this is a constant buffer view root descriptor
	rootParameters[0].Descriptor = rootCBVDescriptor; // this is the root descriptor for this root parameter
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // our pixel shader will be the only shader accessing this parameter for now

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
	rootParameters[1].DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // our pixel shader will be the only shader accessing this parameter for now

	// create a static sampler
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(_countof(rootParameters), // we have 2 root parameters
		rootParameters, // a pointer to the beginning of our root parameters array
		1, // we have one static sampler
		&sampler, // a pointer to our static sampler (array)
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // we can deny shader stages here for better performance
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

	ID3DBlob* signature;
	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (FAILED(hr)) return false;

	hr = _RD->m_d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&s_rootSignature));
	if (FAILED(hr)) return false;

	// create vertex and pixel shaders

	// when debugging, we can compile the shader files at runtime.
	// but for release versions, we can compile the hlsl shaders
	// with fxc.exe to create .cso files, which contain the shader
	// bytecode. We can load the .cso files at runtime to get the
	// shader bytecode, which of course is faster than compiling
	// them at runtime

	// compile vertex shader
	ID3DBlob* vertexShader; // d3d blob for holding vertex shader bytecode
	ID3DBlob* errorBuff; // a buffer holding the error data if any
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string shader_path = m_filepath.generateFilepath("VertexShader", m_filepath.SHADER);
	std::wstring w_shader_path = converter.from_bytes(shader_path.c_str());
	hr = D3DCompileFromFile(w_shader_path.c_str(),
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShader,
		&errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return false;
	}

	// fill out a shader bytecode structure, which is basically just a pointer
	// to the shader bytecode and the size of the shader bytecode
	D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	// compile pixel shader
	ID3DBlob* pixelShader;
	shader_path = m_filepath.generateFilepath("PixelShader", m_filepath.SHADER);
	w_shader_path = converter.from_bytes(shader_path.c_str());
	hr = D3DCompileFromFile(w_shader_path.c_str(),
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
		return false;
	}

	// fill out shader bytecode structure for pixel shader
	D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	// create input layout

	// The input layout is used by the Input Assembler so that it knows
	// how to read the vertex data bound to it.

	// fill out an input layout description structure
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

	// we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
	inputLayoutDesc.NumElements = sizeof(myVertexLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = myVertexLayout;

	// create a pipeline state object (PSO)

	// In a real application, you will have many pso's. for each different shader
	// or different combinations of shaders, different blend states or different rasterizer states,
	// different topology types (point, line, triangle, patch), or a different number
	// of render targets you will need a pso

	// VS is the only required shader for a pso. You might be wondering when a case would be where
	// you only set the VS. It's possible that you have a pso that only outputs data with the stream
	// output, and not on a render target, which means you would not need anything after the stream
	// output.

	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	 // a structure to define a pso
	s_psoDesc.InputLayout = inputLayoutDesc; // the structure describing our input layout
	s_psoDesc.pRootSignature = s_rootSignature; // the root signature that describes the input data this pso needs
	s_psoDesc.VS = vertexShaderBytecode; // structure describing where to find the vertex shader bytecode and how large it is
	s_psoDesc.PS = pixelShaderBytecode; // same as VS but for pixel shader
	s_psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
	s_psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM; // format of the render target
	s_psoDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
	s_psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
	D3D12_RASTERIZER_DESC RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	RS.CullMode = D3D12_CULL_MODE_FRONT;
	s_psoDesc.RasterizerState = RS; // a default(ish) rasterizer state.
	s_psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blend state.
	s_psoDesc.NumRenderTargets = 1; // we are only binding one render target
	s_psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // a depth stencil state
	s_psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	
	// create the pso
	hr = _RD->m_d3dDevice->CreateGraphicsPipelineState(&s_psoDesc, IID_PPV_ARGS(&s_pipelineStateObject));
	if (FAILED(hr))
	{
		return false;
	}	

	//load base texture white
	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	std::string image_path = m_filepath.generateFilepath("white", m_filepath.IMAGE);
	std::wstring w_image_path = converter.from_bytes(image_path.c_str());
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(_RD->m_d3dDevice.Get(), resourceUpload, w_image_path.c_str(), &s_texture));

	auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

	uploadResourcesFinished.wait();


	return true;
}

void VBGO3D::UpdateConstBuff(RenderData * _RD)
{
	//update Constant buffer
	m_ConstBuff.world = m_world.Transpose();
	int uj = 1;
	m_ConstBuff.view = _RD->m_cam->GetView().Transpose();
	m_ConstBuff.projection = _RD->m_cam->GetProj().Transpose();
	//for (int i = 0; i < num_of_cam; i++)
	//{
	//	m_ConstBuff.view = _RD->m_cam[i]->GetView().Transpose();
	//	m_ConstBuff.projection = _RD->m_cam[i]->GetProj().Transpose();
	//}
	m_ConstBuff.rot = m_rot.Transpose();
	if (_RD->m_light)
	{
		m_ConstBuff.lightCol = _RD->m_light->GetColour();
		m_ConstBuff.lightPos = _RD->m_light->GetPos();
		m_ConstBuff.ambientCol = _RD->m_light->GetAmbCol();
	}

	// copy our ConstantBuffer instance to the mapped constant buffer resource
	memcpy(m_ConstBuffGPUAddress[*_RD->m_backBufferIndex], &m_ConstBuff, sizeof(m_ConstBuff));

	_RD->m_commandList->SetGraphicsRootConstantBufferView(0, m_constantBufferUploadHeap[*_RD->m_backBufferIndex]->GetGPUVirtualAddress());

	// set constant buffer descriptor heap
	//ID3D12DescriptorHeap* descriptorHeaps[] = { m_mainDescriptorHeap[*_RD->m_backBufferIndex] };
	//_RD->m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void VBGO3D::BuildIB(RenderData* _RD, int _numI, void * _indices)
{
	int iBufferSize = sizeof(DWORD)*_numI;

	// create default heap to hold index buffer
	_RD->m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_COPY_DEST, // start in the copy destination state
		nullptr, // optimized clear value must be null for this type of resource
		IID_PPV_ARGS(&m_indexBuffer));

	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	m_indexBuffer->SetName(L"Index Buffer Resource Heap");

	// create upload heap to upload index buffer
	ID3D12Resource* iBufferUploadHeap;
	_RD->m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
		nullptr,
		IID_PPV_ARGS(&iBufferUploadHeap));
	iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	// store Index buffer in upload heap
	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(_indices); // pointer to our index array
	indexData.RowPitch = iBufferSize; // size of all our index buffer
	indexData.SlicePitch = iBufferSize; // also the size of our index buffer

										// we are now creating a command with the command list to copy the data from
										// the upload heap to the default heap
	UpdateSubresources(_RD->m_commandList.Get(), m_indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);

	// transition the index buffer data from copy destination state to vertex buffer state
	_RD->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.Format = DXGI_FORMAT_R32_UINT; // 32-bit unsigned integer (this is what a dword is, double word, a word is 2 bytes)
	m_indexBufferView.SizeInBytes = iBufferSize;
}

void VBGO3D::BuildVB(RenderData* _RD, int _numVerts, void * _vertices)
{
	int vBufferSize = sizeof(myVertex) * _numVerts;

	// create default heap
	// default heap is memory on the GPU. Only the GPU has access to this memory
	// To get data into this heap, we will have to upload the data using
	// an upload heap
	_RD->m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
										// from the upload heap to this heap
		nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
		IID_PPV_ARGS(&m_vertexBuffer));

	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	m_vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	// create upload heap
	// upload heaps are used to upload data to the GPU. CPU can write to it, GPU can read from it
	// We will upload the vertex buffer using this heap to the default heap
	ID3D12Resource* vBufferUploadHeap;
	_RD->m_d3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
		nullptr,
		IID_PPV_ARGS(&vBufferUploadHeap));
	vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(_vertices); // pointer to our vertex array
	vertexData.RowPitch = vBufferSize; // size of all our triangle vertex data
	vertexData.SlicePitch = vBufferSize; // also the size of our triangle vertex data

										 // we are now creating a command within the command list to copy the data from
										 // the upload heap to the default heap
	UpdateSubresources(_RD->m_commandList.Get(), m_vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);

	// transition the vertex buffer data from copy destination state to vertex buffer state
	_RD->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(myVertex);
	m_vertexBufferView.SizeInBytes = vBufferSize;
}

void VBGO3D::PushIBVB(RenderData * _RD)
{
	// increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
	_RD->m_fenceValues[*_RD->m_backBufferIndex]++;
	HRESULT hr = _RD->m_commandQueue->Signal(_RD->m_fence.Get(), _RD->m_fenceValues[*_RD->m_backBufferIndex]);

	// Now we execute the command list to upload the initial assets
	_RD->m_commandList->Close();
	ID3D12CommandList* ppCommandLists[] = { _RD->m_commandList.Get() };
	_RD->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void VBGO3D::CalcNorms(myVertex* _verts, DWORD* _indices, int _numI)
{
	for (int i = 0; i<_numI / 3; i++)
	{
		DWORD V1 = _indices[3 * i];
		DWORD V2 = _indices[3 * i + 1];
		DWORD V3 = _indices[3 * i + 2];

		//build normals
		Vector3 norm;
		Vector3 vec1 = _verts[V1].Pos - _verts[V2].Pos;
		Vector3 vec2 = _verts[V3].Pos - _verts[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		_verts[V1].Norm = norm;
		_verts[V2].Norm = norm;
		_verts[V3].Norm = norm;
	}
}


