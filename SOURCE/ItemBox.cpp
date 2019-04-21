#include "pch.h"
#include "ItemBox.h"
#include "ServiceLocator.h"
#include "Player.h"
#include "ItemData.h"
#include "Constants.h"
#include "GameStateData.h"

ItemBox::ItemBox(Vector3& position, Vector3& rotation) : PhysModel("ITEM_BOX") {
	SetPos(position);
	SetRotation(rotation);

	m_physData.m_additionalScale = 50;
	DebugText::print("CREATING ITEM BOX AT: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");
}

void ItemBox::hasCollided(Player* collided_player) {
	setVisible(false);
	collided_player->setItemInInventory(Locator::getItemData()->GetRandomItem(collided_player->GetRanking()));
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