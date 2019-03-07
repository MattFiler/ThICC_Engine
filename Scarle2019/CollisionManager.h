#pragma once
#include "Collision.h"
#include <vector>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	static std::vector<Collision> checkPhysModelCollisions(std::vector<PhysModel*> _physModel);
private:

};

