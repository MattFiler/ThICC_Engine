#pragma once

#include "GameFilepaths.h"
#include "ItemMesh.h"
#include "ServiceLocator.h"
#include "ItemData.h"
#include "AnimationController.h"
#include <json.hpp>
using json = nlohmann::json;

class Player;

class Item
{
public:
	Item() = default;
	Item(ItemType _type);
	void InitItemData(const std::string & item_type);
	~Item() = default;
	
	virtual void Tick();
	virtual void Render();
	virtual void HitByPlayer(Player* _player) = 0;
	virtual void Use(Player* _player, bool _altUse) = 0;

	bool ShouldDestroy() { return m_shouldDestroy; };
	void FlagForDestoy() { m_shouldDestroy = true; };

	TrackMagnet* GetMesh() { return m_itemMesh->m_mesh.get(); };

	virtual void AddImmuneItem(Item* _item) { m_immuneItems.push_back(_item); };
	virtual void addImmuneItems(std::vector<Item*> _immuneItems) { m_immuneItems = _immuneItems; };
	virtual std::vector<Item*> GetImmuneItems() { return m_immuneItems; };
	virtual void setTrailingPlayerImmunity(bool _immunity) { m_trailingPlayerImmunity = _immunity; };

	Player* getPlayer() { return m_player; };
	void setPlayer(Player* _player) { m_player = _player; };

	float getSpinAngle() { return m_spinAngle; };
	void setSpinAngle(float _angle);

	bool isTrailing() { return m_trailing; };
	void setTrailing(bool _trailing) { m_trailing = _trailing; };

	ItemType GetItemType() { return m_itemType; };
	ItemMesh* GetItemMesh() { return m_itemMesh; };

protected:
	Player* m_player = nullptr;

	json m_itemData;
	ItemType m_itemType;
	ItemMesh* m_itemMesh = nullptr;
	bool m_shouldDestroy = false;
	bool m_itemUsed = false;
	bool m_shouldDespawn = true;
	bool m_trailingPlayerImmunity = true;
	bool m_trailing = false;

	//Despawn Time
	float m_elapsedTime = 0;
	float m_maxDuration = 0;

	//Player Immunity time
	float m_elapsedImmunityTime = 0;
	float m_maxImmunityTime = 0;

	//Angle of orbit around player
	float m_spinAngle = 0;

	//Vector of items that are immune to this items collisions - needed for triple items
	std::vector<Item*> m_immuneItems;

	virtual bool ignorePlayerCollisions(Player* player);
	virtual void setItemInUse(Player* player);
};