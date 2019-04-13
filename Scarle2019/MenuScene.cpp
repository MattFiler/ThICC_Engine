#include "pch.h"
#include "MenuScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

MenuScene::MenuScene()
{
	m_scene_manager = new SceneManager();
}

MenuScene::~MenuScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
}

void MenuScene::Update()
{
	if (m_key.keyPressed("Quit"))
	{
		ExitGame();
	}
	else if (Locator::getGSD()->m_keyboardState.Enter || Locator::getGSD()->m_gamePadState[0].IsStartPressed())
	{
		m_scene_manager->setCurrentScene(Scenes::GAMESCENE);
		Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP)->Stop();
	}

	if (game_start)
	{
		Locator::getAudio()->Play(SOUND_TYPE::MENU, (int)SOUNDS_MENU::TITLE_START);
		game_start = false;
	}

	//in splash screen
	enterPlayerLobby();

	if (m_menu_state == States::NOSTATE)
	{
		timeout -= Locator::getGSD()->m_dt;

		if (timeout < 0 && intro_music_start)
		{
			Locator::getAudio()->Play(SOUND_TYPE::MENU, (int)SOUNDS_MENU::TTLE_LOOP);
			intro_music_start = false;
		}
	}
	//in lobby
	else if (m_menu_state == States::LOBBY)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_charTimeout[i] > 0)
			{
				m_charTimeout[i] -= Locator::getGSD()->m_dt;
			}
		}
		playerJoin();
	}

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	if (m_key.keyPressed("Load Debug Scene"))
	{
		m_scene_manager->setCurrentScene(Scenes::DEBUGSCENE);
	}
}

void MenuScene::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{
	m_commandList->RSSetViewports(1, &Locator::getWD()->men_viewport);
	m_commandList->RSSetScissorRects(1, &Locator::getWD()->men_scissorRect);
	Locator::getRD()->m_cam = m_cam;

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Render();
	}

	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_resourceDescriptors->Heap()};
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	m_commandList->RSSetViewports(1, &Locator::getWD()->sprite_viewport);
	m_commandList->RSSetScissorRects(1, &Locator::getWD()->sprite_rect);
	Locator::getRD()->m_spriteBatch->SetViewport(Locator::getWD()->sprite_viewport);
	Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());

	//draw 2d objects
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render();
	}
	Locator::getRD()->m_spriteBatch->End();
}

bool MenuScene::Load()
{
	create2DObjects();
	create3DObjects();
	VBGO3D::PushIBVB(); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds
	pushBackObjects();

	return true;
}

void MenuScene::create2DObjects()
{
	//test text
	Text2D* m_enterMenu = new Text2D("Lewis is not cool.");
	m_2DObjects.push_back(m_enterMenu);

	ImageGO2D* splash_screen = new ImageGO2D("cbc04-jdryd");
	m_2DObjects.push_back(splash_screen);

	ImageGO2D* lobby_screen = new ImageGO2D("lobby");
	lobby_screen->SetPos(Vector2(0, -720));
	m_2DObjects.push_back(lobby_screen);

	//Charecter images
	initCharecterImages();
}

void MenuScene::create3DObjects()
{
	for (int i = 0; i < num_of_cam; i++) {
		//Create a player and position on track
		//player[i] = new Player(_RD, "Standard Kart", i, *_ID->m_gamePad.get());
		//player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		//m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam = new Camera(Locator::getWD()->m_outputWidth, Locator::getWD()->m_outputHeight, 1.0f, 2000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
		m_cam->SetBehav(Camera::BEHAVIOUR::DEBUG_CAM);
		m_3DObjects.push_back(m_cam);

		//Create a viewport
		float width_mod = 0.5f;
		float height_mod = 0.5f;
		switch (i) {
		case 0: {
			*&Locator::getWD()->men_viewport = { 0.0f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth), static_cast<float>(Locator::getWD()->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&Locator::getWD()->men_scissorRect = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight) };
			break;
		}
		//case 1: {
		//	_WD->m_viewport[i] = { static_cast<float>(*&_WD->m_outputWidth) * 0.5f, 0.0f, static_cast<float>(*&_WD->m_outputWidth)* 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth),(int)(_WD->m_outputHeight * 0.5f) };
		//	break;
		//}
		//case 2: {
		//	*&_WD->m_viewport[i] = { 0.0f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	*&_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth * 0.5f),(int)(*&_WD->m_outputHeight) };
		//	break;
		//}
		//case 3: {
		//	*&_WD->m_viewport[i] = { static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	*&_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth),(int)(*&_WD->m_outputHeight) };
		//	break;
		//}
		}
	}
}

void MenuScene::pushBackObjects()
{
	for (int i = 0; i < m_3DObjects.size(); i++)
	{
		if (dynamic_cast<PhysModel*>(m_3DObjects[i]) && dynamic_cast<PhysModel*>(m_3DObjects[i])->hasCollider())
		{
			m_physModels.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i]));
			m_3DObjects.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i])->getDebugCollider());
		}
	}
}

void MenuScene::enterPlayerLobby()
{
	if (Locator::getGSD()->m_gamePadState[0].IsConnected())
	{
		if (Locator::getGSD()->m_gamePadState[0].IsAPressed() && m_menu_state == States::NOSTATE)
		{
			m_menu_state = States::LOBBY;
			m_2DObjects[1]->SetPos(Vector2(0, -720));
			m_2DObjects[2]->SetPos(Vector2(0, 0));
			//m_charecter_images[0][0]->SetPos(Vector2(200, 200));
			Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::TTLE_LOOP)->Stop();
			Locator::getAudio()->Play(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP);
		}

		if (Locator::getGSD()->m_gamePadState[0].IsBPressed() && m_menu_state == States::LOBBY)
		{
			intro_music_start = true;
			timeout = 1.2f;
			game_start = true;
			Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP)->Stop();
			m_menu_state = States::NOSTATE;
			m_2DObjects[1]->SetPos(Vector2(0, 0));
			m_2DObjects[2]->SetPos(Vector2(0, -720));
			resetCharecterImagePos();
		}
	}
}

void MenuScene::playerJoin()
{
	for (int i = 0; i < 4; ++i)
	{
		if (Locator::getGSD()->m_gamePadState[i].IsConnected())
		{
			if (Locator::getGSD()->m_gamePadState[i].IsAPressed() && m_menu_state == States::LOBBY)
			{
				//set player charecter selection image
				m_charecter_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));
			}

			if (m_charTimeout[i] <= 0) // stops continuous flipping of charecter selection
			{
				if (Locator::getGSD()->m_gamePadState[i].thumbSticks.leftX > 0)
				{
					m_charecter_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(0, -500)); //set old image pos
					++Locator::getGSD()->charecter_selected[i];

					if (Locator::getGSD()->charecter_selected[i] > 3)
					{
						Locator::getGSD()->charecter_selected[i] = 0;
					}
					Locator::getAudio()->Play(SOUND_TYPE::CHARACTER_SEL, Locator::getGSD()->charecter_selected[i]);
					m_charTimeout[i] = 0.3f; // set charecter selection timeout
					m_charecter_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));//set new image pos
				}

				if (Locator::getGSD()->m_gamePadState[i].thumbSticks.leftX < 0)
				{
					m_charecter_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(0, -500)); //set old image pos
					--Locator::getGSD()->charecter_selected[i];
					if (Locator::getGSD()->charecter_selected[i] < 0)
					{
						m_2DObjects[3 + i]->SetPos(Vector2(0, -500));//set old image pos
						Locator::getGSD()->charecter_selected[i] = 3;
					}
					Locator::getAudio()->Play(SOUND_TYPE::CHARACTER_SEL, Locator::getGSD()->charecter_selected[i]);
					m_charTimeout[i] = 0.2f; // set charecter selection timeout
					m_charecter_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));//set new image pos
				}
			}
		}
	}
}

void MenuScene::initCharecterImages()
{
	for (int i = 0; i < 4; ++i)
	{
		m_charecter_images[i][0] = new ImageGO2D("MARIO");
		m_charecter_images[i][0]->SetPos(Vector2(0, -500));
		m_charecter_images[i][1] = new ImageGO2D("BOWSER");
		m_charecter_images[i][1]->SetPos(Vector2(0, -500));
		m_charecter_images[i][2] = new ImageGO2D("PEACH");
		m_charecter_images[i][2]->SetPos(Vector2(0, -500));
		m_charecter_images[i][3] = new ImageGO2D("WALUIGI");
		m_charecter_images[i][3]->SetPos(Vector2(0, -500));
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_2DObjects.push_back(m_charecter_images[i][j]);
		}
	}
}

void MenuScene::resetCharecterImagePos()
{
	for (int i = 0; i < 4; ++i)
	{
		m_charecter_images[i][0]->SetPos(Vector2(0, -500));
		m_charecter_images[i][1]->SetPos(Vector2(0, -500));
		m_charecter_images[i][2]->SetPos(Vector2(0, -500));
		m_charecter_images[i][3]->SetPos(Vector2(0, -500));
	}
}

