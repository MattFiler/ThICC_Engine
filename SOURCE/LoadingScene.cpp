#include "pch.h"
#include "LoadingScene.h"
#include "RaceManager.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "GameStateData.h"

LoadingScene::LoadingScene()
{
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
	m_background = new ImageGO2D("LOADING");
}

void LoadingScene::Update(DX::StepTimer const & timer)
{
	time += 1;

	if (time > 30.0f)
	{
		time = 0.0f;
		if (Locator::getRM()->is_cup)
		{
			if (Locator::getRM()->GetMaps().size() == Locator::getRM()->current_race_number)
			{
				//Return to menu
				Locator::getRM()->Reset();
				m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
			}
			else 
			{
				//Advance to next map in cup
				m_scene_manager->setCurrentScene(Scenes::GAMESCENE + Locator::getRM()->GetMaps()[Locator::getRM()->current_race_number]);
			}
		}
		else
		{
			//Return to menu
			Locator::getRM()->Reset();
			m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
		}
	}

}

void LoadingScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& m_commandList)
{
	m_background->Render();
}
