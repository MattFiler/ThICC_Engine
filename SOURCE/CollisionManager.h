#pragma once
#include "Collision.h"
#include "Player.h"
#include "Item.h"
#include <vector>

class CollisionManager
{
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	static  void CollisionDetectionAndResponse(std::vector<PhysModel*> _physModel, std::vector<Item*> _items);
private:
	static std::vector<Collision> CheckPhysModelCollisions(std::vector<PhysModel*> _physModels);
	static void CheckResolveItemCollisions(std::vector<PhysModel*> _physModels, std::vector<Item*> _items);
	static bool CheckItemImmunity(Item* _item1, Item* _item2);

	static void PlayerCollisions(Collision & collision);
	static void ItemBoxCollision(PhysModel*& _player, PhysModel*& _itemBox);
	static void ExplosionCollision(PhysModel*& _player, PhysModel*& _explosion);

	static Plane getPlane(Vector3 _corner1, Vector3 _corner2, float height);
};

