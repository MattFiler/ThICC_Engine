#include "pch.h"
#include "SDKMeshGO3D.h"
#include "RenderData.h"
#include "DebugScene.h"
#include <codecvt>

extern void ExitGame();

/* Create! */
DebugScene::DebugScene() {
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
}

/* Destroy! */
DebugScene::~DebugScene()
{
	m_3DObjects.clear();
	delete debug_cam;
	debug_cam = nullptr;
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool DebugScene::Load()
{
	#ifdef _DEBUG
	create3DObjects();
	#endif
	return true;
}

/* Populate the expensive things! */
void DebugScene::ExpensiveLoad() {
	#ifdef _DEBUG
	DebugText::print("Entering Debug Scene!");

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	Locator::getRD()->current_cubemap_radiance = converter.from_bytes(m_filepaths.generateFilepath("sponza", m_filepaths.CUBEMAP_RADIANCE).c_str());
	Locator::getRD()->current_cubemap_irradiance = converter.from_bytes(m_filepaths.generateFilepath("sponza", m_filepaths.CUBEMAP_IRRADIANCE).c_str());

	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Load();
	}

	debug_cam->SetType(CameraType::DEBUG_CAM);
	#endif
}

/* Unpopulate the expensive things. */
void DebugScene::ExpensiveUnload() {
	#ifdef _DEBUG
	DebugText::print("Leaving Debug Scene!");

	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Reset();
	}

	debug_cam->Reset();
	#endif
}

/* Create all 3D objects in the scene. */
void DebugScene::create3DObjects()
{
	m_3DObjects.push_back(new SDKMeshGO3D("sponza"));
	debug_cam = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, Vector3(0.0f, 3.0f, 10.0f), nullptr, CameraType::DEBUG_CAM);
}

/* Update the scene */
void DebugScene::Update(DX::StepTimer const& timer)
{
	#ifdef _DEBUG
	Locator::getRD()->m_cam = debug_cam;
	debug_cam->Tick();

	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick();
	}
	#else
	m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
	#endif
}

/* Render the 3D scene */
void DebugScene::Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	#ifdef _DEBUG
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		if ((*it)->isVisible()) {
			(*it)->Render();
		}
	}
	#endif
}