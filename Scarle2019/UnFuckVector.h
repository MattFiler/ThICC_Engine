#pragma once

#include "SimpleMath.h"

class UnFuckVector {
public:
	Vector3 UnFuck(Vector3 _fucked) {
		return Vector3(_fucked.x, _fucked.z, _fucked.y * -1);
	}
};