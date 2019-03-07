#include "pch.h"
#include "CollisionManager.h"


CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

/*Checks all physModels in the vector to see if they're inside one another. 
If true it creates a collision struct with the two models in the collision and adds it to a vector.
It also sets the phymodels collisions to true. Returns the vector of collisions*/
std::vector<Collision> CollisionManager::checkPhysModelCollisions(std::vector<PhysModel*> _physModels)
{
	std::vector<Collision> collisions;

	for (PhysModel* physModel1 : _physModels)
	{
		for (PhysModel* physModel2 : _physModels)
		{
			if (physModel1 != physModel2 && physModel1->getCollider().Intersects(physModel2->getCollider()))
			{
				Collision collision;
				physModel1->addCurrentCollision(collision);
				physModel2->addCurrentCollision(collision);

				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}



