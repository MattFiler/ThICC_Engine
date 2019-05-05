#include "pch.h"
#include "ServiceLocator.h"

ThICC_InputData* Locator::ref_ID = nullptr;
ThICC_DeviceData* Locator::ref_DD = nullptr;
ThICC_RenderData* Locator::ref_RD = nullptr;
ThICC_GameStateData* Locator::ref_GSD = nullptr;
ItemData* Locator::ref_ItemProbability = nullptr;
AIScheduler* Locator::ref_AIS = nullptr;
AudioManager* Locator::ref_audio = nullptr;
SceneManager* Locator::ref_SM = nullptr;
GameObjectShared* Locator::ref_GOS = nullptr;
ThICC_CameraData* Locator::ref_CamD = nullptr;
ItemPools* Locator::ref_ItemPools = nullptr;