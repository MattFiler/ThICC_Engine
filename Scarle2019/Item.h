#pragma once

#include "GameFilepaths.h"
#include "TrackMagnet.h"
#include "ServiceLocator.h"
#include "ItemData.h"

class Player;

class Item 
{
public:
	Item() = default;
	Item(const std::string& item_type);
	~Item() = default;
	
	virtual void Tick();
	virtual void HitByPlayer(Player* player) = 0;
	virtual void Use(Player* player, bool _altUse) = 0;

	bool ShouldDestroy() { return m_shouldDestroy; };
	void FlagForDestoy() { m_shouldDestroy = true; };

	TrackMagnet* GetMesh() { return m_mesh; };


protected:
	TrackMagnet * m_mesh = nullptr;
	bool m_shouldDestroy = false;
};