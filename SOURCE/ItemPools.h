#pragma once
#include "ItemMesh.h"
#include <queue>
#include <map>
#include <json.hpp>
using json = nlohmann::json;

class ItemPools
{
public:
	ItemPools();
	~ItemPools();

	void Load();

	ItemMesh* GetItemMesh(ItemType _type);
	void AddItemMesh(ItemType _type, ItemMesh* _mesh);

	AnimationController* GetExplosion(ItemType _type);
	void AddExplosion(AnimationController* _explosion, ItemType _type);

	void Reset();
private:
	void CreateItemPool(ItemType _type, int _poolSize);
	AnimationController * CreateExplosion(ItemType _type);
	ItemMesh* CreateItemMesh(ItemType _type);

	struct ItemPool
	{
		int m_size = 0;
		std::queue<ItemMesh*> m_itemMeshes;
	};

	std::map<ItemType, ItemPool> m_itemPoolMap;
	std::queue<AnimationController*> m_bombExplosions;
	std::queue<AnimationController*> m_shellExplosions;
};

