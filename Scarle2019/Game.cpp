#include "pch.h"
#include "Game.h"
#include "RenderData.h"
#include "GameStateData.h"
#include "CollisionManager.h"
#include "GameDebugToggles.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

bool GameDebugToggles::show_debug_meshes = true;

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
	//Make sure our assets are compiled at least to some degree!
	if (!dirExists("DATA")) {
		throw "ASSETS MUST BE COMPILED BEFORE RUNNING THE GAME";
	}

	//Initialise our DirectX systems
	initDX(_window, _width, _height);

	//Set our default font
	setDefaultFont("Perpetua");
	
	//Configure our controller 
	m_gamePad = std::make_unique<GamePad>();

	//Set Up VBGO render system
	if (!VBGO3D::SetUpVBGOs(m_RD))
	{
		ExitGame();//if anything fails in setting up, QUIT!
	}

	//Configure localisation
	m_localiser.configure("ENGLISH"); //todo: read in from a launcher

	//Setup keybinds
	m_keybinds.setup(m_GSD);

	//Create all GameObjects
	createAllObjects2D();
	createAllObjects3D();

	//Setup the viewport
	setupViewport(_width, _height);

	//debug: output our current directory
	std::cout << std::experimental::filesystem::current_path();

	//Push back all our game objects to their associated arrays
	pushBackObjects();
}

/* Create all 2D game objects */
void Game::createAllObjects2D()
{
	//test text
	Text2D *test2 = new Text2D(m_localiser.getString("debug_text"));
	m_2DObjects.push_back(test2);

	//text example 2D objects
	//ImageGO2D *test = new ImageGO2D(m_RD, "twist");
	//test->SetOri(45);
	//test->SetPos(Vector2(200, 300));
	//test->CentreOrigin();
	//test->SetScale(0.5f*Vector2::One);
	//m_2DObjects.push_back(test);
	//test = new ImageGO2D(m_RD, "guides_logo");
	//test->SetPos(Vector2(100, 100));
	//test->SetScale(Vector2(5.0f, 0.5f));
	//test->SetColour(Color(1, 0, 0, 1));
	//m_2DObjects.push_back(test);

	//GUI TEST
	//Enter in order: LapPos / RacePos / BoxPos / ItemPos
	Vector2 uiPositions[4] = { Vector2(32.f,300.f), Vector2(32.f,350.f),
		Vector2(32.f,32.f), Vector2(32.f,32.f) };
	//RaceUI * player1Test = new RaceUI(m_RD, uiPositions);
	//player1Test->SetPos(Vector2(0, 0));
	//m_2DObjects.push_back(player1Test);

	//Test Sounds
	Loop *loop = new Loop(m_audEngine.get(), "Course Intro Soundtrack");
	loop->SetVolume(0.1f);
	//loop->Play();
	m_sounds.push_back(loop);

	TestSound* TS = new TestSound(m_audEngine.get(), "Explo1");
	m_sounds.push_back(TS);
}

/* Setup our viewport */
void Game::setupViewport(int _width, int _height)
{
	//SetViewport(1, 0.0f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f);
	//SetViewport(0, 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight) * 0.5);

	m_viewport[0] = { 0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH }; //uncommented
	m_scissorRect[0] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight) };

	//m_viewport[0] = { 0.0f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	//m_scissorRect[0] = { 0,0,(int)(m_outputWidth * 0.5f),(int)(m_outputHeight * 0.5f) };
	//m_viewport[1] = { static_cast<float>(m_outputWidth) * 0.5f, 0.0f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	//m_scissorRect[1] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight * 0.5f) };
	//m_viewport[2] = { 0.0f, static_cast<float>(m_outputHeight) * 0.5f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	//m_scissorRect[2] = { 0,0,(int)(m_outputWidth * 0.5f),(int)(m_outputHeight) };
	//m_viewport[3] = { static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, static_cast<float>(m_outputWidth) * 0.5f, static_cast<float>(m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	//m_scissorRect[3] = { 0,0,(int)(m_outputWidth),(int)(m_outputHeight) };

	//point a camera at the player that follows
	m_cam[0] = new Camera(_width / 2, _height / 2, 1.0f, 1000.0f, player[0], Vector3(0.0f, 3.0f, 10.0f));
	m_cam[0]->SetBehav(Camera::BEHAVIOUR::LERP);
	m_3DObjects.push_back(m_cam[0]);

	m_cam[1] = new Camera(_width / 2, _height / 2, 1.0f, 1000.0f, player[0], Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[1]->SetTarget(Vector3(0.0f, 3.0f, 100.0f));
	m_cam[1]->SetBehav(Camera::BEHAVIOUR::LERP);
	m_3DObjects.push_back(m_cam[1]);

	m_cam[2] = new Camera(_width / 2, _height / 2, 1.0f, 1000.0f, player[0], Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[2]->SetTarget(Vector3(0.0f, 10.0f, 200.0f));
	m_cam[2]->SetBehav(Camera::BEHAVIOUR::LERP);
	m_3DObjects.push_back(m_cam[2]);

	m_cam[3] = new Camera(_width / 2, _height / 2, 1.0f, 1000.0f, player[0], Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[3]->SetTarget(Vector3(0.0f, -10.0f, 5.0f));
	m_cam[3]->SetBehav(Camera::BEHAVIOUR::LERP);
	m_3DObjects.push_back(m_cam[3]);
}

/* Create all 3d game objects */
void Game::createAllObjects3D()
{
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

	//Load in a track
	//track = new Track(m_RD, "GBA Mario Circuit");
	track = new Track(m_RD, "Mario Kart Stadium");
	//track = new Track(m_RD, "Luigi Circuit");
	//track = new Track(m_RD, "Driftway");
	m_3DObjects.push_back(track);

	//Create a player and position on track
	player[0] = new Player(m_RD, "Standard Kart", 0, *m_gamePad.get());
	player[0]->SetPos(track->getSuitableSpawnSpot());
	//player[0]->SetPos({0, 0, 0});
	m_3DObjects.push_back(player[0]);

	player[1] = new Player(m_RD, "Standard Kart", 1, *m_gamePad.get());
	player[1]->SetPos(Vector3(player[0]->GetPos().x, player[0]->GetPos().y, player[0]->GetPos().z - 10));
	//player[0]->SetPos({0, 0, 0});
	m_3DObjects.push_back(player[1]);

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
}

/* Push misc objects around */
void Game::pushBackObjects()
{
	//Push our TXTMESHes to the GPU - do not delete this!
	VBGO3D::PushIBVB(m_RD);

	//Add all 3D game objects with a collider to the collision manager's list
	for (int i = 0; i < m_3DObjects.size(); i++) 
{
		if (dynamic_cast<PhysModel*>(m_3DObjects[i]) && dynamic_cast<PhysModel*>(m_3DObjects[i])->hasCollider()) 
		{		
			m_physModels.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i]));
			m_3DObjects.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i])->getDebugCollider());		
		}
	}
}

/* Update is called once per frame */
void Game::Update(DX::StepTimer const& _timer)
{
	// Test code
	for (int i = 0; i < 1; i++)
	{
		player[0]->ShouldStickToTrack(*track, m_GSD);
		player[0]->ResolveWallCollisions(*track);
		player[1]->ShouldStickToTrack(*track, m_GSD);
		player[1]->ResolveWallCollisions(*track);
	}
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

	for (int i = 0; i < num_of_players; ++i)
	{
		m_GSD->m_gamePadState[i] = m_gamePad->GetState(i); //set game controllers state[s]
	}

	//Quit Properly on press ESC
	if (m_keybinds.keyPressed("Quit"))
	{
		ExitGame();
	}
	if (m_keybinds.keyPressed("Orbit"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::ORBIT);
	}
	if (m_keybinds.keyPressed("Lerp"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::LERP);
	}
	if (m_keybinds.keyPressed("Matt"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::MATT_CAM);
	}
	
	//Add your game logic here.
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick(m_GSD);
	}
	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick(m_GSD);
	}

	//Toggle debug mesh renders
	if (m_keybinds.keyPressed("Debug Toggle"))
	{
		GameDebugToggles::show_debug_meshes = !GameDebugToggles::show_debug_meshes;
	}


	CollisionManager::checkPhysModelCollisions(m_physModels);
}

/* render the scene */
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
	for (int i = 0; i < num_of_cam; i++)
	{
		m_RD->m_spriteBatch->SetViewport(m_viewport[i]);
		m_RD->m_spriteBatch->Begin(m_commandList.Get());

		for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
		{
			(*it)->Render(m_RD);
		}

		m_RD->m_spriteBatch->End();
	}
	// Show the new frame.
	Present();
	m_graphicsMemory->Commit(m_commandQueue.Get());
}

/* configure window size */
void Game::GetDefaultSize(int& _width, int& _height) const
{
	_width = 1280;
	_height = 720;
}

/* setup a viewport */
void Game::SetViewport(int i_, float _TopLeftX, float _TopLeftY, float _Width, float _Height)
{
	m_viewport[i_] = { _TopLeftX,_TopLeftY,_Width,_Height, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	m_scissorRect[i_] = { (int)_TopLeftX,(int)_TopLeftY,(int)(_Width),(int)(_Height) };
}