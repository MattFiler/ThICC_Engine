//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "RenderData.h"
#include "GameStateData.h"


extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;


Game::Game() :
	m_window(nullptr),
	m_outputWidth(1000),
	m_outputHeight(1000),
	m_featureLevel(D3D_FEATURE_LEVEL_11_0),
	m_backBufferIndex(0),
	m_fenceValues{}
{
}

Game::~Game()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}

	// Ensure that the GPU is no longer referencing resources that are about to be destroyed.
	WaitForGpu();

	//delete the GO2Ds
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		delete (*it);
	}
	m_2DObjects.clear();

	//delete the GO3Ds
	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		delete (*it);
	}
	m_3DObjects.clear();

	//delete the sounds
	for (vector<Sound *>::iterator it = m_sounds.begin(); it != m_sounds.end(); it++)
	{
		delete (*it);
	}
	m_sounds.clear();

	delete m_RD;
	delete m_GSD;

	VBGO3D::CleanUp();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
	m_window = _window;
	m_outputWidth = std::max(_width, 1);
	m_outputHeight = std::max(_height, 1);
	m_RD = new RenderData;

	CreateDevice();
	CreateResources();

	//set up input stuff
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(_window);// mouse device needs to linked to this program's window
	m_mouse->SetMode(Mouse::Mode::MODE_RELATIVE); // gives a delta postion as opposed to a MODE_ABSOLUTE position in 2-D space

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);

	//Create the Game State Data object
	m_GSD = new GameStateData;

	//GEP: Set up RenderData Object
	m_RD->m_d3dDevice = m_d3dDevice;
	m_RD->m_commandQueue = m_commandQueue;
	m_RD->m_commandList = m_commandList;
	for (int i = 0; i < c_swapBufferCount; i++)
	{
		m_RD->m_commandAllocators[i] = m_commandAllocators[i];
	}	
	m_RD->m_fenceValues = m_fenceValues;
	m_RD->m_fence = m_fence;
	m_RD->m_backBufferIndex = &m_backBufferIndex;

	m_RD->m_resourceDescriptors = std::make_unique<DescriptorHeap>(m_d3dDevice.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		100);

	//GEP: Set up Sprite batch for drawing textures also loads the font for text
	m_RD->m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	ResourceUploadBatch resourceUpload(m_d3dDevice.Get());

	resourceUpload.Begin();
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	SpriteBatchPipelineStateDescription pd(rtState);
	pd.blendDesc = m_RD->m_states->NonPremultiplied;
	m_RD->m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dDevice.Get(), resourceUpload, pd);
	m_RD->m_font = std::make_unique<SpriteFont>(m_d3dDevice.Get(), resourceUpload,
		L"courier.spritefont",
		m_RD->m_resourceDescriptors->GetCpuHandle(m_RD->m_resourceCount),
		m_RD->m_resourceDescriptors->GetGpuHandle(m_RD->m_resourceCount));
	m_RD->m_resourceCount++;

	auto uploadResourcesFinished = resourceUpload.End(m_commandQueue.Get());

	//SetViewport(1, 0.0f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f);
	//SetViewport(0, 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight) * 0.5);
	//m_viewport[0] = { 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	//m_scissorRect[0] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight) };
	m_viewport[0] = { 0.0f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[0] = { 0,0,(int)(m_outputWidth * 0.5f),(int)(m_outputHeight * 0.5f) };
	m_viewport[1] = { static_cast<float>(m_outputWidth) * 0.5f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[1] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight * 0.5f) };
	m_viewport[2] = { 0.0f, static_cast<float>(m_outputHeight) * 0.5f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[2] = { 0,0,(int)(m_outputWidth * 0.5f),(int)(m_outputHeight) };
	m_viewport[3] = { static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[3] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight) };


	//Set Up VBGO render system
	if (!VBGO3D::SetUpVBGOs(m_RD))
	{
		ExitGame();//if anything fails in setting up QUIT!
	}

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	//GEP::This is where I am creating the test objects


	//geometric shape renderer test
	for (int i = 1; i < GP_COUNT; i++)
	{
		//GPGO3D* test3d2 = new GPGO3D((GPGO3D_Type)i);
		//test3d2->SetPos(12.0f*Vector3::Forward + 10.0f*(i - 1)*Vector3::Left);
		//test3d2->SetScale(5.0f);
		//m_3DObjects.push_back(test3d2);
	}

	//test for obj loader / renderer
	//SDKMeshGO3D *test3 = new SDKMeshGO3D(m_RD, "Luigi Circuit");
	//test3->SetPos(Vector3(0, -30, 0));
	//test3->SetScale(10.0f);
	//test3->SetRotationInDegrees(Vector3(0, 0, 0));
	//m_3DObjects.push_back(test3);

	//create a "player"
	player = new Player(m_RD, "Kart");
	player->SetPos(Vector(-345, 555.0f, 350));
	//player->SetRotationInDegrees(Vector3(180, 180, 180));
	m_3DObjects.push_back(player);

	// Test track
	track = new Track(m_RD, "Rainbow Road");
	//m_3DObjects.push_back(track);
	//SDKMeshGO3D* track = new SDKMeshGO3D(m_RD, "Test Track");
	m_3DObjects.push_back(track);

	//point a camera at the player that follows
	m_cam[0] =  new Camera(static_cast<float>(100), static_cast<float>(90), 1.0f, 1000.0f, player, Vector3(0.0f, 3.0f, 10.0f));
	//m_RD->m_cam = m_cam[0];
	m_3DObjects.push_back(m_cam[0]);

	m_cam[1] = new Camera(static_cast<float>(100), static_cast<float>(90), 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	m_cam[1]->SetTarget(Vector3(0.0f, 3.0f, 100.0f));
	//m_RD->m_cam = m_cam[1];
	m_3DObjects.push_back(m_cam[1]);

	m_cam[2] = new Camera(static_cast<float>(100), static_cast<float>(90), 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	m_cam[2]->SetTarget(Vector3(0.0f, 10.0f, 200.0f));
	//m_RD->m_cam = m_cam[1];
	m_3DObjects.push_back(m_cam[2]);

	m_cam[3] = new Camera(static_cast<float>(100), static_cast<float>(90), 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	m_cam[3]->SetTarget(Vector3(0.0f, -10.0f, 5.0f));
	//m_RD->m_cam = m_cam[1];
	m_3DObjects.push_back(m_cam[3]);

	//create a base light
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	m_RD->m_light = m_light;

	/*/test VBGO3Ds
	VBCube* VB = new VBCube(m_RD);
	VB->Init(11, m_RD);
	VB->SetScale(4.0f);
	m_3DObjects.push_back(VB);
	VBSpiral* VB2 = new VBSpiral(m_RD);
	VB2->Init(11, m_RD);
	VB2->SetPos(100.0f*Vector3::Forward);
	VB2->SetScale(4.0f);
	m_3DObjects.push_back(VB2);
	VBSpike* VB3 = new VBSpike(m_RD);
	VB3->Init(11, m_RD);
	VB3->SetPos(100.0f*Vector3::Backward);
	VB3->SetScale(4.0f);
	m_3DObjects.push_back(VB3); */

	/*/Marching Cubes
	VBMarchCubes* VBMC = new VBMarchCubes(m_RD);
	VBMC->init(Vector3(-8.0f, -8.0f, -17.0f), Vector3(8.0f, 8.0f, 23.0f), 60.0f*Vector3::One, 0.01, m_RD);
	VBMC->SetPos(Vector3(100, 0, -100));
	VBMC->SetPitch(-XM_PIDIV2);
	VBMC->SetScale(Vector3(3, 3, 1.5));
	m_3DObjects.push_back(VBMC);

	FileVBGO* terrainBox = new FileVBGO("terrainTex", m_RD);
	terrainBox->SetPos(Vector3(100.0f, 000.0f, 100.0f));
	m_3DObjects.push_back(terrainBox);

	FileVBGO* Box = new FileVBGO("cube", m_RD);
	m_3DObjects.push_back(Box);
	Box->SetPos(Vector3(200.0f, 0.0f, 200.0f));
	Box->SetPitch(XM_PIDIV4);
	Box->SetScale(20.0f);

	VBSnail* snail = new VBSnail(m_RD, "baseline", 150, 0.98f, 0.09f * XM_PI, 0.4f, Color(1.0f, 0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f, 1.0f));
	snail->SetPos(Vector3(100.0f, 0.0f, 0.0f));
	snail->SetScale(2.0f);
	m_3DObjects.push_back(snail); */

	//this only draws correctly as its at the end
	//see the file for what's needed to do this properly
	//TransFileVBGO* Box2 = new TransFileVBGO("cube_trans", m_RD);
	//m_3DObjects.push_back(Box2);
	//Box2->SetPos(Vector3(0.0f, 50.0f, 0.0f));
	//Box2->SetPitch(0.5f*XM_PIDIV4);
	//Box2->SetScale(20.0f);

	//Once I.ve set up all the VBs and IBs push them to the GPU
	VBGO3D::PushIBVB(m_RD); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds

	//test text
	Text2D * test2 = new Text2D("testing text");
	m_2DObjects.push_back(test2);

	//text example 2D objects
	ImageGO2D *test = new ImageGO2D(m_RD, "twist");
	test->SetOri(45);
	test->SetPos(Vector2(200, 300));
	test->CentreOrigin();
	test->SetScale(0.5f*Vector2::One);
	m_2DObjects.push_back(test);
	test = new ImageGO2D(m_RD, "guides_logo");
	test->SetPos(Vector2(100, 100));
	test->SetScale(Vector2(1.0f, 0.5f));
	test->SetColour(Color(1, 0, 0, 1));
	m_2DObjects.push_back(test);

	//Test Sounds
	//Loop *loop = new Loop(m_audEngine.get(), "Course Intro Soundtrack");
	//loop->SetVolume(0.1f);
	//loop->Play();
	//m_sounds.push_back(loop);

	//TestSound* TS = new TestSound(m_audEngine.get(), "Explo1");
	//m_sounds.push_back(TS);
}

// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
	// Test code
	player->ShouldStickToTrack(*track);
	m_GSD->m_dt = float(_timer.GetElapsedSeconds());

	//this will update the audio engine but give us chance to do somehting else if that isn't working
	if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			// We lost the audio device!
		}
	}
	else
	{
		//update sounds playing
		for (vector<Sound *>::iterator it = m_sounds.begin(); it != m_sounds.end(); it++)
		{
			(*it)->Tick(m_GSD);
		}
	}

	//Poll Keyboard and Mouse
	//More details here: https://github.com/Microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
	//You can find out how to set up controllers here: https://github.com/Microsoft/DirectXTK/wiki/Game-controller-input
	m_GSD->m_prevKeyboardState = m_GSD->m_keyboardState; // keep previous state for just pressed logic
	m_GSD->m_keyboardState = m_keyboard->GetState();
	m_GSD->m_mouseState = m_mouse->GetState();

	//Quit Properly on press ESC
	if (m_GSD->m_keyboardState.Escape)
	{
		ExitGame();
	}

	//if (m_GSD->m_keyboardState.Left)
	//{
	//	m_RD->m_cam = m_cam;
	//}
	//if (m_GSD->m_keyboardState.Right)
	//{
	//	m_RD->m_cam = m_cam1;
	//}

	//Add your game logic here.
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick(m_GSD);
	}
	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick(m_GSD);
	}
}

// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	// Prepare the command list to render a new frame.
		Clear();

	for (int i = 0; i < num_of_cam; i++)
	{
		m_commandList->RSSetViewports(1, &m_viewport[i]);
		m_commandList->RSSetScissorRects(1, &m_scissorRect[i]);
		m_RD->m_cam = m_cam[i];
		//draw 3D objects
		for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
		{
			(*it)->Render(m_RD);
		}
	}

	//finally draw all 2D objects
	ID3D12DescriptorHeap* heaps[] = { m_RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_RD->m_spriteBatch->SetViewport(m_viewport[0]);
	//m_RD->m_spriteBatch->SetViewport(m_viewport[1]);
	m_RD->m_spriteBatch->Begin(m_commandList.Get());

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render(m_RD);
	}

	m_RD->m_spriteBatch->End();

	// Show the new frame.
	Present();
	m_graphicsMemory->Commit(m_commandQueue.Get());
}

// Helper method to prepare the command list for rendering and clear the back buffers.
void Game::Clear()
{
	// Reset command list and allocator.
	DX::ThrowIfFailed(m_commandAllocators[m_backBufferIndex]->Reset());
	DX::ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_backBufferIndex].Get(), nullptr));

	// Transition the render target into the correct state to allow for drawing into it.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &barrier);

	// Clear the views.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvDescriptor(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	m_commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	m_commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
	m_commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
}

// Submits the command list to the GPU and presents the back buffer contents to the screen.
void Game::Present()
{
	// Transition the render target to the state that allows it to be presented to the display.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &barrier);

	// Send the command list off to the GPU for processing.
	DX::ThrowIfFailed(m_commandList->Close());
	m_commandQueue->ExecuteCommandLists(1, CommandListCast(m_commandList.GetAddressOf()));

	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);

		MoveToNextFrame();
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
	m_audEngine->Suspend();
}

void Game::OnResuming()
{
	// TODO: Game is being power-resumed (or returning from minimize).
	m_timer.ResetElapsedTime();
	m_audEngine->Resume();
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
	m_outputWidth = std::max(_width, 1);
	m_outputHeight = std::max(_height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	_width = 800;
	_height = 800;
}

void Game::SetViewport(int i_, float _TopLeftX, float _TopLeftY, float _Width, float _Height)
{
	m_viewport[i_] = { _TopLeftX,_TopLeftY,_Width,_Height, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[i_] = { (int)_TopLeftX,(int)_TopLeftY,(int)(_Width),(int)(_Height) };
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	DWORD dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	//
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
		{
			debugController->EnableDebugLayer();
		}

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
	}
#endif

	DX::ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

	ComPtr<IDXGIAdapter1> adapter;
	GetAdapter(adapter.GetAddressOf());

	// Create the DX12 API device object.
	DX::ThrowIfFailed(D3D12CreateDevice(
		adapter.Get(),
		m_featureLevel,
		IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf())
	));

#ifndef NDEBUG
	// Configure debug device (if active).
	ComPtr<ID3D12InfoQueue> d3dInfoQueue;
	if (SUCCEEDED(m_d3dDevice.As(&d3dInfoQueue)))
	{
#ifdef _DEBUG
		d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
		D3D12_MESSAGE_ID hide[] =
		{
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
		};
		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;
		d3dInfoQueue->AddStorageFilterEntries(&filter);
	}
#endif

	// Create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	DX::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf())));

	// Create descriptor heaps for render target views and depth stencil views.
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
	rtvDescriptorHeapDesc.NumDescriptors = c_swapBufferCount;
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
	dsvDescriptorHeapDesc.NumDescriptors = 1;
	dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(m_rtvDescriptorHeap.ReleaseAndGetAddressOf())));
	DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(m_dsvDescriptorHeap.ReleaseAndGetAddressOf())));

	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Create a command allocator for each back buffer that will be rendered to.
	for (UINT n = 0; n < c_swapBufferCount; n++)
	{
		DX::ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocators[n].ReleaseAndGetAddressOf())));
	}

	// Create a command list for recording graphics commands.
	DX::ThrowIfFailed(m_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf())));
	//DX::ThrowIfFailed(m_commandList->Close());

	// Create a fence for tracking GPU execution progress.
	DX::ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));
	m_fenceValues[m_backBufferIndex]++;

	m_fenceEvent.Attach(CreateEvent(nullptr, FALSE, FALSE, nullptr));
	if (!m_fenceEvent.IsValid())
	{
		throw std::exception("CreateEvent");
	}

	//Initialize device dependent objects here (independent of window size).
	m_graphicsMemory = std::make_unique<GraphicsMemory>(m_d3dDevice.Get());

	//set up things for the geometric shape renderer
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	EffectPipelineStateDescription pd(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState);

	m_RD->m_GPeffect = std::make_unique<BasicEffect>(m_d3dDevice.Get(), EffectFlags::Lighting, pd);
	m_RD->m_GPeffect->EnableDefaultLighting();


	//set up things for the obj model loader / renderer
	EffectPipelineStateDescription pd3(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullNone,
		rtState);
	m_RD->m_GPeffect = std::make_unique<BasicEffect>(m_d3dDevice.Get(), EffectFlags::Lighting, pd3);
	m_RD->m_GPeffect->EnableDefaultLighting();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	// Wait until all previous GPU work is complete.
	WaitForGpu();

	// Release resources that are tied to the swap chain and update fence values.
	for (UINT n = 0; n < c_swapBufferCount; n++)
	{
		m_renderTargets[n].Reset();
		m_fenceValues[n] = m_fenceValues[m_backBufferIndex];
	}

	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT;
	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(c_swapBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = c_swapBufferCount;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a swap chain for the window.
		ComPtr<IDXGISwapChain1> swapChain;
		DX::ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			swapChain.GetAddressOf()
		));

		DX::ThrowIfFailed(swapChain.As(&m_swapChain));

	}

	// Obtain the back buffers for this window which will be the final render targets
	// and create render target views for each of them.
	for (UINT n = 0; n < c_swapBufferCount; n++)
	{
		DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(m_renderTargets[n].GetAddressOf())));

		wchar_t name[25] = {};
		swprintf_s(name, L"Render target %u", n);
		m_renderTargets[n]->SetName(name);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), n, m_rtvDescriptorSize);
		m_d3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvDescriptor);
	}

	// Reset the index to the current back buffer.
	m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
	// on this surface.
	CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		depthBufferFormat,
		backBufferWidth,
		backBufferHeight,
		1, // This depth stencil view has only one texture.
		1  // Use a single mipmap level.
	);
	depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = depthBufferFormat;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	DX::ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
		&depthHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(m_depthStencil.ReleaseAndGetAddressOf())
	));

	m_depthStencil->SetName(L"Depth stencil");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = depthBufferFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// TODO: Initialize windows-size dependent objects here.
}

void Game::WaitForGpu() noexcept
{
	if (m_commandQueue && m_fence && m_fenceEvent.IsValid())
	{
		// Schedule a Signal command in the GPU queue.
		UINT64 fenceValue = m_fenceValues[m_backBufferIndex];
		if (SUCCEEDED(m_commandQueue->Signal(m_fence.Get(), fenceValue)))
		{
			// Wait until the Signal has been processed.
			if (SUCCEEDED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent.Get())))
			{
				WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);

				// Increment the fence value for the current frame.
				m_fenceValues[m_backBufferIndex]++;
			}
		}
	}
}

void Game::MoveToNextFrame()
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = m_fenceValues[m_backBufferIndex];
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// Update the back buffer index.
	m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (m_fence->GetCompletedValue() < m_fenceValues[m_backBufferIndex])
	{
		DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_backBufferIndex], m_fenceEvent.Get()));
		WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	m_fenceValues[m_backBufferIndex] = currentFenceValue + 1;
}

// This method acquires the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, try WARP. Otherwise throw an exception.
void Game::GetAdapter(IDXGIAdapter1** _ppAdapter)
{
	*_ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf()); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		DX::ThrowIfFailed(adapter->GetDesc1(&desc));

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_featureLevel, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

#if !defined(NDEBUG)
	if (!adapter)
	{
		if (FAILED(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
		{
			throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
		}
	}
#endif

	if (!adapter)
	{
		throw std::exception("No Direct3D 12 device found");
	}

	*_ppAdapter = adapter.Detach();
}

void Game::OnDeviceLost()
{
	//reset DKTK stuff
	m_RD->m_font.reset();
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Reset();
	}

	//Geometric renderer

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		if ((*it)->GetType() == GO3D_RT_GEOP || (*it)->GetType() == GO3D_RT_SDK)
		{
			(*it)->Reset();
		}
	}
	m_RD->m_GPeffect.reset();

	//SDK Mesh loader / renderer
	m_RD->m_states.reset();
	m_RD->m_fxFactory.reset();

	m_RD->m_resourceDescriptors.reset();
	m_graphicsMemory.reset();

	//Perform Direct3D resource cleanup.
	for (UINT n = 0; n < c_swapBufferCount; n++)
	{
		m_commandAllocators[n].Reset();
		m_renderTargets[n].Reset();
	}

	m_depthStencil.Reset();
	m_fence.Reset();
	m_commandList.Reset();
	m_swapChain.Reset();
	m_rtvDescriptorHeap.Reset();
	m_dsvDescriptorHeap.Reset();
	m_commandQueue.Reset();
	m_d3dDevice.Reset();
	m_dxgiFactory.Reset();

	CreateDevice();
	CreateResources();
}
