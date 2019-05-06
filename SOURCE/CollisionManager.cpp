#include "pch.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "ItemBox.h"
#include "Bomb.h"
#include "Explosion.h"
#include <iostream>
#include <fstream>


float CollisionManager::m_thresholdDistSqrd = 0;
ItemCollisionData CollisionManager::m_collisionData = ItemCollisionData();

void CollisionManager::InitConfig()
{
	std::ifstream i("DATA/CONFIGS/COLLISION_MANAGER_CONFIG.JSON");
	json data;
	data << i;

	m_thresholdDistSqrd = (float)data["threshold_collision_distance_squared"];
	m_collisionData.m_playerVelMulti = (float)data["invincibility_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)data["invincibility_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)data["invincibility_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)data["invincibility_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)data["invincibility_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)data["invincibility_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)data["invincibility_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)data["invincibility_collision"]["vertical_pos_offset"];
}

void CollisionManager::CollisionDetectionAndResponse(std::vector<PhysModel*> _physModels, std::vector<Item*> _items)
{
	std::vector<Collision> collisions = CheckPhysModelCollisions(_physModels);
	CheckResolveItemCollisions(_physModels, _items);

	for (Collision& collision : collisions)
	{
		if (dynamic_cast<Player*>(collision.m_model1))
		{
			//Player x Player Collision
			if (dynamic_cast<Player*>(collision.m_model2))
			{
				PlayerCollisions(collision.m_model1, collision.m_model2, collision.m_collisionNormal);
			}
			//Player x Item Box Collision
			else if (collision.m_model2->isVisible() && dynamic_cast<ItemBox*>(collision.m_model2))
			{
				ItemBoxCollision(collision.m_model1, collision.m_model2);
			}
			//Player x Explosion Collision
			else if (dynamic_cast<Explosion*>(collision.m_model2))
			{
				ExplosionCollision(collision.m_model1, collision.m_model2);
			}
		}
		else if (dynamic_cast<Player*>(collision.m_model2))
		{
			//Item Box x Player Collision
			if (collision.m_model2->isVisible() && dynamic_cast<ItemBox*>(collision.m_model2))
			{
				ItemBoxCollision(collision.m_model2, collision.m_model1);
			}
			//Explosion x Player Collision
			else if (dynamic_cast<Explosion*>(collision.m_model1))
			{
				ExplosionCollision(collision.m_model2, collision.m_model1);
			}
		}
	}
}

void CollisionManager::ItemBoxCollision(PhysModel*& _player, PhysModel*& _itemBox)
{
	dynamic_cast<ItemBox*>(_itemBox)->hasCollided(dynamic_cast<Player*>(_player));
	Locator::getAudio()->Play(SoundType::MISC, (int)MiscSounds::ITEM_BOX_HIT);
}

void CollisionManager::ExplosionCollision(PhysModel *& _player, PhysModel *& _explosion)
{
	if (!dynamic_cast<Player*>(_player)->isInvincible())
	{
		dynamic_cast<Explosion*>(_explosion)->HitByPlayer(dynamic_cast<Player*>(_player));
	}
}

void CollisionManager::PlayerCollisions(PhysModel*& _player1, PhysModel*& _player2, Vector3 _collisionNormal)
{
	Player* player1 = dynamic_cast<Player*>(_player1);
	Player* player2 = dynamic_cast<Player*>(_player2);

	if (player1->isInvincible() && !player2->isInvincible())
	{
		ApplyInvincibleResponse(player2);
	}
	else if (player2->isInvincible() && !player1->isInvincible())
	{
		ApplyInvincibleResponse(player1);
	}
	else
	{
		Vector3 rv = _player2->getVelocity() - _player1->getVelocity();
		float contactVel = rv.Dot(_collisionNormal);

		if (contactVel > 0)
		{
			return;
		}

		float e = 1;
		float j = -(1.0f + e) * contactVel;
		j /= 1 / _player1->getMass() + 1 / _player2->getMass();

		Vector3 impulse = j * _collisionNormal;

		_player1->setVelocity(_player1->getVelocity() - impulse * (1.0f / _player1->getMass()));
		_player2->setVelocity(_player2->getVelocity() + impulse * (1.0f / _player2->getMass()));

		LightningCloudCollision(player2, player1);
	}
}

void CollisionManager::ApplyInvincibleResponse(Player * player)
{
	player->setVelocity(player->getVelocity() * m_collisionData.m_playerVelMulti);
	player->Jump(m_collisionData.m_jumpHeight, m_collisionData.m_jumpDuration);
	player->Flip(m_collisionData.m_flipRev, m_collisionData.m_flipDuration);
	player->Spin(m_collisionData.m_spinRev, m_collisionData.m_spinDuration);
	player->AddPos(player->GetWorld().Up() * m_collisionData.m_vertPosOffset);
	player->UpdateWorld();
}

void CollisionManager::LightningCloudCollision(Player * player2, Player * player1)
{
	LightningCloud* cloud = player2->GetLightningCloud();
	if (cloud)
	{
		player1->SpawnItems(LIGHTNING_CLOUD);
		player1->GetLightningCloud()->SetElapsedStrikeTime(cloud->GetElapsedStrikeTime());
	}
}

/*Checks all physModels in the vector to see if they're inside one another.
If true it creates a collision struct with the two models in the collision and adds it to a vector.
It also sets the phymodels collisions to true. Returns the vector of collisions*/
std::vector<Collision> CollisionManager::CheckPhysModelCollisions(std::vector<PhysModel*> _physModels)
{
	std::vector<Collision> collisions;

	for (PhysModel* physModel1 : _physModels)
	{
		int count1 = 0;

		for (PhysModel* physModel2 : _physModels)
		{
			if (dynamic_cast<ItemBox*>(physModel1) && dynamic_cast<ItemBox*>(physModel2))
			{
				continue;
			}

			if (physModel1 != physModel2 && Vector3::DistanceSquared(physModel1->GetPos(), physModel2->GetPos()) < m_thresholdDistSqrd
				&& physModel1->getCollider().Intersects(physModel2->getCollider()))
			{
				Collision collision;

				if (dynamic_cast<Player*>(physModel1) && dynamic_cast<Player*>(physModel2))
				{
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
				}

				collision.m_model1 = physModel1;
				collision.m_model2 = physModel2;

				collisions.push_back(collision);
			}
		}
	}

	return collisions;
}

void CollisionManager::CheckResolveItemCollisions(std::vector<PhysModel*> _physModels, std::vector<Item*> _items)
{

	for (Item* item1 : _items)
	{
		if (item1->GetItemMesh())
		{
			bool hit_player = false;

			//Player x Item Collision
			for (PhysModel* model : _physModels)
			{
				Player* player = dynamic_cast<Player*>(model);
				if (player && player != item1->getPlayer() && Vector3::DistanceSquared(item1->GetMesh()->GetPos(), model->GetPos()) < m_thresholdDistSqrd
					&& item1->GetMesh()->getCollider().Intersects(player->getCollider()))
				{
					if (player->isInvincible())
					{
						item1->FlagForDestoy();
					}
					else
					{
						item1->HitByPlayer(player);
					}

					hit_player = true;
					break;
				}
			}

			if (!hit_player)
			{
				//Item x Item Collision
				for (Item* item2 : _items)
				{
					if (item1 != item2 && item2->GetItemMesh() && Vector3::DistanceSquared(item1->GetMesh()->GetPos(), item2->GetMesh()->GetPos()) < m_thresholdDistSqrd
						&& !CheckItemImmunity(item1, item2) && item1->GetMesh()->getCollider().Intersects(item2->GetMesh()->getCollider()))
					{
						//Checking for bombs
						if (bombResponse(item1, item2))
						{
							continue;
						}

						//Checking for fake item boxes
						fakeBoxResponse(item1, item2);

						item1->FlagForDestoy();
						item2->FlagForDestoy();
						break;
					}
				}
			}
		}
	}

}

bool CollisionManager::bombResponse(Item * item1, Item * item2)
{
	Bomb* bomb1 = dynamic_cast<Bomb*>(item1);
	Bomb* bomb2 = dynamic_cast<Bomb*>(item2);

	if (bomb1)
	{
		bomb1->Detonate();
		item2->FlagForDestoy();
	}
	else if (bomb2)
	{
		item1->FlagForDestoy();
		bomb2->Detonate();
	}

	return bomb1 || bomb2;
}

void CollisionManager::fakeBoxResponse(Item * item1, Item * item2)
{
	FakeItemBox* box1 = dynamic_cast<FakeItemBox*>(item1);
	FakeItemBox* box2 = dynamic_cast<FakeItemBox*>(item2);

	if (box1 && box1->isTrailing())
	{
		box1->HitByPlayer(box1->getPlayer());
	}
	else if (box2 && box2->isTrailing())
	{
		box2->HitByPlayer(box2->getPlayer());
	}
}

bool CollisionManager::CheckItemImmunity(Item * _item1, Item * _item2)
{
	for (Item* item : _item1->GetImmuneItems())
	{
		if (item == _item2)
		{
			return true;
		}
	}

	for (Item* item : _item2->GetImmuneItems())
	{
		if (item == _item1)
		{
			return true;
		}
	}

	return false;
}

Plane CollisionManager::getPlane(Vector3 _corner1, Vector3 _corner2, float _height)
{
	Vector3 side = _corner1 - _corner2;
	Vector3 endPoint = _corner1 + side;
	Vector3 distance = Vector3(_corner1.x, _corner1.y + _height, _corner1.z);
	return Plane(_corner1, endPoint, distance);
}