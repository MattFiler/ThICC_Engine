#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;
class SceneManager;

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	void Update() override;
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList) override;
	bool Load() override;

private:

	void create2DObjects();
	void create3DObjects();
	void pushBackObjects();
	void enterPlayerLobby();
	void playerJoin();
	void initCharecterImages();
	void resetCharecterImagePos();

	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects
	Camera*												m_cam;
	Light*												m_light;

	int num_of_cam = 1;

	std::vector<PhysModel*> m_physModels;

	json game_config;
	LocalisationManager m_localiser;
	GameFilepaths m_filepath;
	KeybindManager m_key;
	SceneManager* m_scene_manager;
	States m_menu_state = States::NOSTATE;

	//Charecter Images
	ImageGO2D* m_charecter_images[4][4]; //[player][charecter]

	//Timeout for charecter selection
	float m_charTimeout[4] = {0.0f,0.0f,0.0f,0.0f};

	float timeout = 1.2f;
	bool intro_music_start = true;
	bool game_start = true;

};

