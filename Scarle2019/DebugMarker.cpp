#include "pch.h"
#include "DebugMarker.h"

DebugMarker::DebugMarker(Vector3& position, Vector3& rotation) : PhysModel("Cup") {
	SetPos(position);
	SetRotation(rotation);

	std::cout << "CREATING DEBUG MARKER AT: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}