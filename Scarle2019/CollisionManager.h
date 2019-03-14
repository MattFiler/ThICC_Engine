#pragma once
#include "Collision.h"
#include <vector>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	static  void collisionDetectionAndResponse(std::vector<PhysModel*> _physModel);
private:
	static std::vector<Collision> checkPhysModelCollisions(std::vector<PhysModel*> _physModel);
	static Plane getPlane(Vector3 _corner1, Vector3 _corner2, float height);
};

