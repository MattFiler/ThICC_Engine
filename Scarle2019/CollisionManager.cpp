#include "pch.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "ItemBox.h"
#include "Player.h"


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
		//Player collided with item box on track
		if (dynamic_cast<ItemBox*>(collision.m_model1) && dynamic_cast<Player*>(collision.m_model2)) {
			if (collision.m_model1->isVisible()) {
				dynamic_cast<ItemBox*>(collision.m_model1)->hasCollided(dynamic_cast<Player*>(collision.m_model2));
				Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::ITEM_BOX_HIT);
			}
		}
		else if (dynamic_cast<ItemBox*>(collision.m_model2) && dynamic_cast<Player*>(collision.m_model1)) {
			if (collision.m_model2->isVisible()) {
				dynamic_cast<ItemBox*>(collision.m_model2)->hasCollided(dynamic_cast<Player*>(collision.m_model1));
				Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::ITEM_BOX_HIT);
			}
		}
		//Collided with another player
		else {
			Vector3 rv = collision.m_model2->getVelocity() - collision.m_model1->getVelocity();
			float contactVel = rv.Dot(collision.m_collisionNormal);

			if (contactVel > 0)
			{
				continue;
			}

			float e = 1;
			float j = -(1.0f + e) * contactVel;
			j /= 1 / collision.m_model1->getMass() + 1 / collision.m_model2->getMass();

			Vector3 impulse = j * collision.m_collisionNormal;

			collision.m_model1->setVelocity(collision.m_model1->getVelocity() - impulse * (1.0f / collision.m_model1->getMass()));
			collision.m_model2->setVelocity(collision.m_model2->getVelocity() + impulse * (1.0f / collision.m_model2->getMass()));
		}
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
				
				Plane frontPlane = getPlane(physModel2->data.m_globalFrontTopLeft, physModel2->data.m_globalFrontTopRight, physModel2->data.m_height);
				Plane backPlane = getPlane(physModel2->data.m_globalBackTopLeft, physModel2->data.m_globalBackTopRight, physModel2->data.m_height);
				Plane rightPlane = getPlane(physModel2->data.m_globalFrontTopRight, physModel2->data.m_globalBackTopRight, physModel2->data.m_height);
				Plane leftPlane = getPlane(physModel2->data.m_globalFrontTopLeft, physModel2->data.m_globalBackTopLeft, physModel2->data.m_height);

		
				if (physModel1->getCollider().Intersects(backPlane))
				{
					collision.m_collisionNormal = backPlane.Normal();
				}			 
				else if (physModel1->getCollider().Intersects(frontPlane))
				{
					collision.m_collisionNormal = frontPlane.Normal();
				}
				else if (physModel1->getCollider().Intersects(rightPlane))
				{
					collision.m_collisionNormal = leftPlane.Normal();
				}
				else if (physModel1->getCollider().Intersects(leftPlane))
				{
					collision.m_collisionNormal = leftPlane.Normal();
				}
				collision.m_model1 = physModel1;
				collision.m_model2 = physModel2;

				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}

Plane CollisionManager::getPlane(Vector3 _corner1, Vector3 _corner2, float _height)
{
	Vector3 side = _corner1 - _corner2;
	Vector3 endPoint = _corner1 + side;
	Vector3 distance = Vector3(_corner1.x, _corner1.y + _height, _corner1.z);
	return Plane(_corner1, endPoint, distance);
}



