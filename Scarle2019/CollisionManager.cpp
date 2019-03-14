#include "pch.h"
#include "CollisionManager.h"


CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::collisionDetectionAndResponse(std::vector<PhysModel*> _physModels)
{
	std::vector<Collision> collisions = checkPhysModelCollisions(_physModels);

	for (Collision& collision : collisions)	
	{
		Vector3 rv = collision.m_model2->getVelocity() - collision.m_model1->getVelocity();
		float contactVel = rv.Dot(collision.m_collisionNormal);

		if (contactVel > 0)
		{
			continue;
		}

		float e = 0.1;
		float j = -(1.0f + e) * contactVel;
		j /= collision.m_model1->getMass() + collision.m_model2->getMass();

		Vector3 impulse = j * collision.m_collisionNormal;

		collision.m_model1->setVelocityTotal(collision.m_model1->getVelocity() - collision.m_model1->getMass() * impulse);
		collision.m_model2->setVelocityTotal(collision.m_model2->getVelocity() - collision.m_model2->getMass() * impulse);

	}

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
				Plane frontPlane = getPlane(physModel2->getCorner(PhysModel::m_Corner::FRONT_LEFT), physModel2->getCorner(PhysModel::m_Corner::FRONT_RIGHT));
				Plane backPlane = getPlane(physModel2->getCorner(PhysModel::m_Corner::BACK_LEFT), physModel2->getCorner(PhysModel::m_Corner::BACK_RIGHT));
				Plane rightPlane = getPlane(physModel2->getCorner(PhysModel::m_Corner::FRONT_RIGHT), physModel2->getCorner(PhysModel::m_Corner::BACK_RIGHT));
				Plane leftPlane = getPlane(physModel2->getCorner(PhysModel::m_Corner::FRONT_LEFT), physModel2->getCorner(PhysModel::m_Corner::BACK_LEFT));

				if (physModel1->getCollider().Intersects(backPlane))
				{
					collision.m_frontBack = Collision::m_FrontBack::BACK;
					collision.m_collisionNormal = backPlane.Normal();
				}
				 
				if (physModel1->getCollider().Intersects(frontPlane))
				{

					collision.m_frontBack = Collision::m_FrontBack::FRONT;
					collision.m_collisionNormal = frontPlane.Normal();
				}
				
				if (physModel1->getCollider().Intersects(leftPlane))
				{
					collision.m_leftRight = Collision::m_LeftRight::LEFT;
				}

				if (physModel1->getCollider().Intersects(rightPlane))
				{
					collision.m_leftRight = Collision::m_LeftRight::RIGHT;
				}

				collision.m_penertation_depth = physModel1->GetPos() - physModel2->GetPos();
				collision.m_penertation_depth.x = abs(collision.m_penertation_depth.x);
				collision.m_penertation_depth.y = abs(collision.m_penertation_depth.y);
				collision.m_penertation_depth.z = abs(collision.m_penertation_depth.z);

				collision.m_model1 = physModel1;
				collision.m_model2 = physModel2;

				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}

Plane CollisionManager::getPlane(Vector3 _corner1, Vector3 _corner2)
{
	Vector3 side = _corner1 - _corner2;
	Vector3 endPoint = _corner1 + side;
	return Plane(_corner1, endPoint / 2, endPoint);;
}



