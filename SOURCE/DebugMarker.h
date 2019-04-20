#pragma once

#include "PhysModel.h"
#include "SimpleMath.h"
#include <iostream>

class DebugMarker : public PhysModel {
public:
	DebugMarker(Vector3 position, Vector3 rotation);
	~DebugMarker() = default;

private:
};