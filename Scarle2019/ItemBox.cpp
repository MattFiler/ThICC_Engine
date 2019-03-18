#include "pch.h"
#include "ItemBox.h"
#include "ServiceLocator.h"
#include "Player.h"

//Todo: replace "Default Item" with item box model
ItemBox::ItemBox(Vector3& position, Vector3& rotation) : PhysModel("Default Item") {
	SetPos(position);
	SetRotation(rotation);

	std::cout << "CREATING ITEM BOX AT: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void ItemBox::hasCollided(Player* collided_player) {
	setVisible(false);

	//Generate an item here based on probability

	collided_player->setItemInInventory(ItemType::GREEN_SHELL);
}

void ItemBox::Tick() {
	if (!isVisible()) {
		invisibility_timer += Locator::getGSD()->m_dt;
		if (invisibility_timer >= 3.0) {
			setVisible(true);
			invisibility_timer = 0.0;
		}
	}

	PhysModel::Tick();
}