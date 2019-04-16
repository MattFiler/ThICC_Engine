#include "pch.h"
#include "ServiceLocator.h"

ThICC_InputData* Locator::ref_ID = nullptr;
ThICC_RenderData* Locator::ref_RD = nullptr;
ThICC_GameStateData* Locator::ref_GSD = nullptr;
ItemData* Locator::ref_ItemProbability = nullptr;
AIScheduler* Locator::ref_AIS = nullptr;