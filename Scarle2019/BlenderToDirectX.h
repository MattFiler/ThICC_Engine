#pragma once

#include "SimpleMath.h"

class BlenderToDirectX {
public:
	Vector3 ConvertPosition(Vector3 _blender) {
		return Vector3(_blender.x, _blender.z, _blender.y * -1);
	}
};