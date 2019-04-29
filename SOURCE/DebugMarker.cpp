#include "pch.h"
#include "DebugMarker.h"
#include "GameObjectShared.h"

DebugMarker::DebugMarker(Vector3 position, Vector3 rotation) : PhysModel(Locator::getGOS()->common_model_config["debug_marker"]) {
	SetPos(position);
	SetRotation(rotation);

	DebugText::print("CREATING DEBUG MARKER AT: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");
}