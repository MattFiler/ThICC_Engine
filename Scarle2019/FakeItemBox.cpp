#include "pch.h"
#include "FakeItemBox.h"

FakeItemBox::FakeItemBox() : Item(Locator::getItemData()->GetItemModelName(FAKE_BOX))
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
	m_maxImmunityTime = 0.5f;
}

void FakeItemBox::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	if (_altUse)
	{
		//Positions the box
		m_mesh->SetWorld(player->GetWorld());
		m_mesh->AddPos(player->GetWorld().Right() * 2 + player->GetWorld().Up() * 2);
		m_mesh->UpdateWorld();

		//Yeets said box
		float forward_force = 100;
		float upward_force = 1;
		m_mesh->setMaxGrav(500);
		m_mesh->setGravVelocity(player->getVelocity() + (player->GetWorld().Forward() * forward_force) + (player->GetWorld().Up() * upward_force));
	}
}

void FakeItemBox::HitByPlayer(Player * player)
{
	// Prevent collisions with the parent that fired this for a bit
	if (Item::ignorePlayerCollisions(player))
	{
		return;
	}

	player->setVelocity(Vector3::Zero);
	player->Jump(1.5f, 1);
	player->Flip(1, 0.8f);
	player->AddPos(player->GetWorld().Up() * 4);
	m_shouldDestroy = true;
}
