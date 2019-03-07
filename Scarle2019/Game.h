//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "GameFilepaths.h"
#include "KeybindManager.h"
#include "LocalisationManager.h"
#include <vector>
using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;


//
// A basic game implementation that creates a D3D12 device and
// provides a game loop.
//
// The standard Visual Studio 2017 project template for "a project for a Win32 desktop game that uses Direct3D12".
//
// After that NuGet has been used to install the DXTK12 (Direct X 12 Tool Kit) library.
// I strongly suggest you look through the headers of DXTK12 to familiarise yourself with its API.
// You should find that in a directory called packages\directxtk12_desktop_2015.2018.11.20.1\include
// Documentation for this can be found here: https://github.com/Microsoft/DirectXTK12/wiki

class Game
{
public:

	Game();
	~Game();

	// Initialization and management
	void Initialize(HWND _window, int _width, int _height);

	void initDX(const HWND &_window, int &_width, int &_height);

	void setDefaultFont(string _default_font);

	void createAllObjects2D();

	void setupViewport(int _width, int _height);

	void createAllObjects3D();

	void pushBackObjects();

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int _width, int _height);

	// Properties
	void GetDefaultSize(int& _width, int& _height) const;

	void SetViewport(int i_, float _TopLeftX,float _TopLeftY, float _Width, float _Height);

private:

	// Test objects
	Track* track = nullptr;
	Player* player[2] = {nullptr};
	PhysModel* test_model = nullptr;

	void Update(DX::StepTimer const& _timer);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void WaitForGpu() noexcept;
	void MoveToNextFrame();
	void GetAdapter(IDXGIAdapter1** _ppAdapter);

	bool dirExists(const std::string& dirName_in);

	void OnDeviceLost();

	// Application state
	HWND                                                m_window;
	int                                                 m_outputWidth;
	int                                                 m_outputHeight;

	// Direct3D Objects
	D3D_FEATURE_LEVEL                                   m_featureLevel;
	UINT                                                m_backBufferIndex;
	UINT                                                m_rtvDescriptorSize;
	Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
	static const UINT									c_swapBufferCount =2;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
	UINT64                                              m_fenceValues[c_swapBufferCount];
	Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

	D3D12_VIEWPORT										m_viewport[4];
	D3D12_RECT											m_scissorRect[4];

	// Rendering resources
	Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

	//DXTK stuff
	std::unique_ptr<DirectX::GraphicsMemory>			m_graphicsMemory;

	//Input
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::GamePad> m_gamePad;

	//audio system
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	vector<Sound*> m_sounds;

	// Game State
	DX::StepTimer                                       m_timer;
	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	//vector<UI*>											m_UIObjects;
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects
	Camera*												m_cam[4];
	Light*												m_light; 
	RenderData*											m_RD;	//dumping ground for things required to do rendering
	GameStateData*										m_GSD;  //base game state to pass to all GameObjects
	int num_of_cam = 1;
	int num_of_players = 2;

	GameFilepaths m_filepath;
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;

	std::vector<PhysModel*> m_physModels;
};
