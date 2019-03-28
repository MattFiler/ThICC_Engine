#pragma once

#include "GameFilepaths.h"
#include "TrackMagnet.h"
#include "ServiceLocator.h"
#include "ItemData.h"
#include "Deletable.h"
#include "AnimationMesh.h"

class Player;

class Item  : public Deletable
{
public:
	Item() = default;
	Item(const std::string& item_type);
	~Item() = default;
	
	virtual void Tick();
	virtual void HitByPlayer(Player* _player) = 0;
	virtual void Use(Player* _player, bool _altUse) = 0;

	bool ShouldDestroy() { return m_shouldDestroy; };
	void FlagForDestoy() { m_shouldDestroy = true; };

	TrackMagnet* GetMesh() { return m_mesh; };
	SDKMeshGO3D* GetRenderMesh() { return m_displayedMesh.get(); };


protected:
	TrackMagnet * m_mesh = nullptr;
	std::unique_ptr<AnimationMesh> m_displayedMesh = nullptr;
	bool m_shouldDestroy = false;
};