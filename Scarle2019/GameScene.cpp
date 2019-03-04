#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();


GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

Scenes GameScene::Update(GameStateData* _GSD, InputData* _ID)
{
	player[0]->ShouldStickToTrack(*track);

	//Poll Keyboard and Mouse
//More details here: https://github.com/Microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
//You can find out how to set up controllers here: https://github.com/Microsoft/DirectXTK/wiki/Game-controller-input
	_GSD->m_prevKeyboardState = _GSD->m_keyboardState; // keep previous state for just pressed logic
	_GSD->m_keyboardState = _ID->m_keyboard->GetState();
	_GSD->m_mouseState = _ID->m_mouse->GetState();

	for (int i = 0; i < 4; ++i)
	{
		_GSD->m_gamePadState[i] = _ID->m_gamePad->GetState(i); //set game controllers state[s]
	}

	if (_GSD->m_keyboardState.Escape)
	{
		ExitGame();
	}

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick(_GSD);
	}

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick(_GSD);
	}

	return nextScene;
}

void GameScene::Render(RenderData* _RD)
{
	//draw 3D objects

	//camera setup.

	for (int i = 0; i < num_of_cam; ++i)
	{
		_RD->m_cam = m_cam[i];
	}

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Render(_RD);
	}

	//draw 2d objects
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render(_RD);
	}
}

bool GameScene::Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	track = new Track(_RD, "Mario Kart Stadium");
	m_3DObjects.push_back(track);

	//Create a player and position on track
	player[0] = new Player(_RD, "Standard Kart", 0, *_ID->m_gamePad);
	player[0]->SetPos(track->getSuitableSpawnSpot());
	m_3DObjects.push_back(player[0]);

	//point a camera at the player that follows
	m_cam[0] = new Camera(_WD->m_width, _WD->m_height, 1.0f, 1000.0f, player[0], Vector3(0.0f, 3.0f, 10.0f));
	//m_RD->m_cam = m_cam[0];
	m_3DObjects.push_back(m_cam[0]);

	//m_cam[1] = new Camera(_WD->m_width / 2, _WD->m_height / 2, 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[1]->SetTarget(Vector3(0.0f, 3.0f, 100.0f));
	////m_RD->m_cam = m_cam[1];
	//m_3DObjects.push_back(m_cam[1]);

	//m_cam[2] = new Camera(_WD->m_width / 2, _WD->m_height / 2, 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[2]->SetTarget(Vector3(0.0f, 10.0f, 200.0f));
	////m_RD->m_cam = m_cam[1];
	//m_3DObjects.push_back(m_cam[2]);

	//m_cam[3] = new Camera(_WD->m_width / 2, _WD->m_height / 2, 1.0f, 1000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	//m_cam[3]->SetTarget(Vector3(0.0f, -10.0f, 5.0f));
	////m_RD->m_cam = m_cam[1];
	//m_3DObjects.push_back(m_cam[3]);

	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	_RD->m_light = m_light;

	VBGO3D::PushIBVB(_RD); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds

	//debug: output our current directory
	std::cout << std::experimental::filesystem::current_path();

	return false;
}
