#include "pch.h"
#include "MenuScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Text2D.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

/* Create! */
MenuScene::MenuScene()
{
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
}

/* Destroy! */
MenuScene::~MenuScene()
{
	m_2DObjects.clear();
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool MenuScene::Load()
{
	create2DObjects();
	return true;
}

/* Create all 2D objects for the scene */
void MenuScene::create2DObjects()
{
	ImageGO2D* splash_screen = new ImageGO2D("cbc04-jdryd");
	m_2DObjects.push_back(splash_screen);

	//ImageGO2D* lobby_screen = new ImageGO2D("lobby");
	//lobby_screen->SetPos(Vector2(0, -720));
	//m_2DObjects.push_back(lobby_screen);

	//Charecter images
	//initCharacterImages();

	//debug text
	Text2D* m_enterMenu = new Text2D("PRESS ENTER FOR DRAGON DRIFTWAY / D FOR MARIO KART STADIUM");
	m_2DObjects.push_back(m_enterMenu);
}

/* Update the scene */
void MenuScene::Update(DX::StepTimer const& timer)
{
	if (m_keybinds.keyPressed("Quit"))
	{
		ExitGame();
	}
	else if (Locator::getID()->m_keyboardState.Enter || Locator::getID()->m_gamePadState[0].IsStartPressed())
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

	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	if (m_keybinds.keyPressed("Load Debug Scene"))
	{
		m_scene_manager->setCurrentScene(Scenes::GAMESCENE+1);
	}
}

/* Render the 2D scene */
void MenuScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render();
	}
}

/* Player enters into the "lobby" */
void MenuScene::enterPlayerLobby()
{
	if (Locator::getID()->m_gamePadState[0].IsConnected())
	{
		if (Locator::getID()->m_gamePadState[0].IsAPressed() && m_menu_state == States::NOSTATE)
		{
			m_menu_state = States::LOBBY;
			m_2DObjects[1]->SetPos(Vector2(0, -720));
			m_2DObjects[2]->SetPos(Vector2(0, 0));
			//m_character_images[0][0]->SetPos(Vector2(200, 200));
			Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::TTLE_LOOP)->Stop();
			Locator::getAudio()->Play(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP);
		}

		if (Locator::getID()->m_gamePadState[0].IsBPressed() && m_menu_state == States::LOBBY)
		{
			intro_music_start = true;
			timeout = 1.2f;
			game_start = true;
			Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP)->Stop();
			m_menu_state = States::NOSTATE;
			m_2DObjects[1]->SetPos(Vector2(0, 0));
			m_2DObjects[2]->SetPos(Vector2(0, -720));
			resetCharacterImagePos();
		}
	}
}

/* Player joins the "lobby" */
void MenuScene::playerJoin()
{
	for (int i = 0; i < 4; ++i)
	{
		if (Locator::getID()->m_gamePadState[i].IsConnected())
		{
			if (Locator::getID()->m_gamePadState[i].IsAPressed() && m_menu_state == States::LOBBY)
			{
				//set player charecter selection image
				m_character_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));
			}

			if (m_charTimeout[i] <= 0) // stops continuous flipping of charecter selection
			{
				if (Locator::getID()->m_gamePadState[i].thumbSticks.leftX > 0)
				{
					m_character_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(0, -500)); //set old image pos
					++Locator::getGSD()->charecter_selected[i];

					if (Locator::getGSD()->charecter_selected[i] > 3)
					{
						Locator::getGSD()->charecter_selected[i] = 0;
					}
					Locator::getAudio()->Play(SOUND_TYPE::CHARACTER_SEL, Locator::getGSD()->charecter_selected[i]);
					m_charTimeout[i] = 0.3f; // set charecter selection timeout
					m_character_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));//set new image pos
				}

				if (Locator::getID()->m_gamePadState[i].thumbSticks.leftX < 0)
				{
					m_character_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(0, -500)); //set old image pos
					--Locator::getGSD()->charecter_selected[i];
					if (Locator::getGSD()->charecter_selected[i] < 0)
					{
						m_2DObjects[3 + i]->SetPos(Vector2(0, -500));//set old image pos
						Locator::getGSD()->charecter_selected[i] = 3;
					}
					Locator::getAudio()->Play(SOUND_TYPE::CHARACTER_SEL, Locator::getGSD()->charecter_selected[i]);
					m_charTimeout[i] = 0.2f; // set charecter selection timeout
					m_character_images[i][Locator::getGSD()->charecter_selected[i]]->SetPos(Vector2(50 + (i * 300), 100));//set new image pos
				}
			}
		}
	}
}

/* Initialise the images for each character */
void MenuScene::initCharacterImages()
{
	for (int i = 0; i < 4; ++i)
	{
		m_character_images[i][0] = new ImageGO2D("MARIO");
		m_character_images[i][0]->SetPos(Vector2(0, -500));
		m_character_images[i][1] = new ImageGO2D("BOWSER");
		m_character_images[i][1]->SetPos(Vector2(0, -500));
		m_character_images[i][2] = new ImageGO2D("PEACH");
		m_character_images[i][2]->SetPos(Vector2(0, -500));
		m_character_images[i][3] = new ImageGO2D("WALUIGI");
		m_character_images[i][3]->SetPos(Vector2(0, -500));
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_2DObjects.push_back(m_character_images[i][j]);
		}
	}
}

/* reset the position of each character image */
void MenuScene::resetCharacterImagePos()
{
	for (int i = 0; i < 4; ++i)
	{
		m_character_images[i][0]->SetPos(Vector2(0, -500));
		m_character_images[i][1]->SetPos(Vector2(0, -500));
		m_character_images[i][2]->SetPos(Vector2(0, -500));
		m_character_images[i][3]->SetPos(Vector2(0, -500));
	}
}

