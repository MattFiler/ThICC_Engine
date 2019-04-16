#pragma once

#include "SimpleMath.h"

class BlenderToDirectX {
public:
	Vector3 ConvertPosition(Vector3 _blender) {
		return Vector3(_blender.x, _blender.z, _blender.y * -1);
	}
	Vector3 ConvertAngle(Vector3 _blender) {
		return Vector3(-_blender.x + 1.5708, _blender.y * -1, -_blender.z);
	}
};