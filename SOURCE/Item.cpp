#include "pch.h"
#include "Item.h"
#include "GameStateData.h"
#include <iostream>
#include <fstream>

Item::Item(const std::string& item_type)
{
	//Set model name
	m_mesh = new TrackMagnet(item_type);
	m_mesh->SetShouldRender(false);
	m_displayedMesh = std::make_unique<AnimationController>();
	m_displayedMesh->AddModel("item", item_type, Vector3::Zero);
	m_displayedMesh->AddModelSet("default", std::vector<std::string>{"item"});
	m_displayedMesh->SwitchModelSet("default");
	m_displayedMesh->Load();

	InitItemData(item_type);
}

void Item::InitItemData(const std::string & item_type)
{
	
		std::string item_name = item_type;
		item_name.erase(item_name.begin(), item_name.begin() + 5); //Removing "ITEM_"

		if (item_name == Locator::getItemData()->GetItemModelName(FAKE_BOX))
		{
			item_name = "FAKE_BOX";
		}

		std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
		m_itemData << i;
		if (item_type != Locator::getItemData()->GetItemModelName(LIGHTNING_CLOUD))
		{
			m_maxDuration = (float)m_itemData[item_name]["info"]["lifetime"];
			m_maxImmunityTime = (float)m_itemData[item_name]["info"]["player_immunity_time"];
		}
}

void Item::Render()
{
	if (m_displayedMesh)
	{
		m_displayedMesh->Render();
	}
}

void Item::Tick()
{
	if (m_mesh)
	{
		m_mesh->Tick();
		m_displayedMesh->Update(m_mesh->GetWorld(), m_mesh->GetWorld().Forward());
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
