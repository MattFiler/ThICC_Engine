#pragma once

#include "MapInfo.h"
#include "VehicleInfo.h"
#include "CharacterInfo.h"
#include <vector>

//Shared game object instance data
struct GameObjectShared {
	std::vector<MapInfo> map_instances;
	std::vector<VehicleInfo> vehicle_instances;
	std::vector<CharacterInfo> character_instances;
};