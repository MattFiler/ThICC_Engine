#include "pch.h"
#include "ItemBox.h"
#include "ServiceLocator.h"
#include "Player.h"
#include "ItemData.h"
#include "Constants.h"
#include "GameStateData.h"
#include "GameObjectShared.h"

ItemBox::ItemBox(Vector3& position, Vector3& rotation) : PhysModel(Locator::getGOS()->common_model_config["itembox"]) {
	SetPos(position);
	SetRotation(rotation);

	m_physData.m_additionalScale = 50;
}

void ItemBox::hasCollided(Player* collided_player) {
	setVisible(false);
	collided_player->SetItemInInventory(Locator::getItemData()->GetRandomItem(collided_player->GetRanking()));
}

void ItemBox::Tick() {
	if (!isVisible()) {
		invisibility_timer += Locator::getGSD()->m_dt;
		if (invisibility_timer >= ItemBoxConfig::respawn_time) {
			setVisible(true);
			invisibility_timer = 0.0;
		}
	}


	PhysModel::Tick();
}