#pragma once
#include <vector>

class MapInfo;
class VehicleInfo;
class CharacterInfo;
class CupInfo;

//Shared game object instance data
struct GameObjectShared {
	std::vector<MapInfo*> map_instances;
	std::vector<VehicleInfo*> vehicle_instances;
	std::vector<CharacterInfo*> character_instances;
	std::vector<CupInfo*> cup_instances;
};