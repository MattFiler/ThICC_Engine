#include "pch.h"
#include "Item.h"
#include "GameStateData.h"
#include "ItemPools.h"
#include <iostream>
#include <fstream>

Item::Item(ItemType _type) : m_itemType(_type)
{
	m_itemMesh = Locator::getItemPools()->GetItemMesh(_type);
	InitItemData(Locator::getItemData()->GetItemModelName(_type));
}

void Item::InitItemData(const std::string & item_type)
{
	std::string itemName = item_type;
	if (itemName == Locator::getItemData()->GetItemModelName(FAKE_BOX))
	{
		itemName = "FAKE_BOX";
	}
	else
	{
		itemName.erase(itemName.begin(), itemName.begin() + 5); //Removing "ITEM_"
	}
	
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	if (item_type != Locator::getItemData()->GetItemModelName(LIGHTNING_CLOUD))
	{
		m_maxDuration = (float)m_itemData[itemName]["info"]["lifetime"];
		m_maxImmunityTime = (float)m_itemData[itemName]["info"]["player_immunity_time"];
	}
}

void Item::Render()
{
	if (m_itemMesh->m_displayedMesh)
	{
		m_itemMesh->m_displayedMesh->Render();
	}
}

void Item::Tick()
{
	if (m_itemMesh->m_mesh)
	{
		m_itemMesh->m_mesh->Tick();
		m_itemMesh->m_displayedMesh->Update(m_itemMesh->m_mesh->GetWorld());
	}

	if (m_itemUsed)
	{
		if (m_shouldDespawn)
		{
			//Despawn time
			m_elapsedTime += Locator::getGSD()->m_dt;
			if (m_elapsedTime > m_maxDuration)
			{
				m_shouldDestroy = true;
			}
		}

		//Player immunity time
		if (m_player && !m_trailingPlayerImmunity)
		{
			DebugText::print("ITEM ELAPSED IMMUNITY TIME: " + std::to_string(m_elapsedImmunityTime));
			m_elapsedImmunityTime += Locator::getGSD()->m_dt;
			if (m_elapsedImmunityTime >= m_maxImmunityTime)
			{
				m_player = nullptr;
			}
		}
	}
}

void Item::setSpinAngle(float _angle)
{
	m_spinAngle = _angle;
	if (m_spinAngle > 360)
	{
		m_spinAngle -= 360;
	}
}

bool Item::ignorePlayerCollisions(Player* player)
{
	return m_player == player;
}

void Item::setItemInUse(Player* player)
{
	m_itemUsed = true;
	m_trailingPlayerImmunity = false;
	m_player = player;
}
