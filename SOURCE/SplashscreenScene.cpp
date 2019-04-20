#include "pch.h"
#include "SplashscreenScene.h"
#include "SceneManager.h"
#include "GameStateData.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

/* Create! */
SplashscreenScene::SplashscreenScene()
{
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
}

/* Destroy! */
SplashscreenScene::~SplashscreenScene()
{
	m_2DObjects.clear();
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool SplashscreenScene::Load()
{
	create2DObjects();
	return true;
}

/* Create all 2D objects for the scene */
void SplashscreenScene::create2DObjects()
{
	m_2DObjects.push_back(new ImageGO2D("WHITE_720"));
	m_logo = new ImageGO2D("engine_logo_white_720");
	m_logo->SetScale(Vector2(0.3, 0.3));
	m_logo->SetPos(Vector2(Locator::getRD()->m_window_width / 2.f, Locator::getRD()->m_window_height / 2.f));
	m_logo->CentreOrigin();
	m_2DObjects.push_back(m_logo);
}

/* Update the scene */
void SplashscreenScene::Update(DX::StepTimer const& timer)
{
	m_logo->SetScale(Vector2(0.3 + (m_timer / 30), 0.3 + (m_timer / 30)));
	m_timer += (float)timer.GetElapsedSeconds();
	if (m_timer > m_timeout) {
		m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
	}
}

/* Render the 2D scene */
void SplashscreenScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render();
	}
}
