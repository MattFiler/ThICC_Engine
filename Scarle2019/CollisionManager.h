#pragma once
#include "Collision.h"
#include <vector>

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	static  void collisionDetectionAndResponse(std::vector<PhysModel*> _physModel);
private:
	static std::vector<Collision> checkPhysModelCollisions(std::vector<PhysModel*> _physModel);
	static void PlayerCollisions(Collision & collision);
	static void ItemBoxCollision(PhysModel*& _player, PhysModel*& _itemBox);

	static Plane getPlane(Vector3 _corner1, Vector3 _corner2, float height);
};

