#include "pch.h"
#include "RedShell.h"
#include "AIScheduler.h"
#include "Player.h"


RedShell::RedShell() : Item(Locator::getItemData()->GetItemModelName(RED_SHELL))
{
	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);

	m_maxImmunityTime = 0.5;
	m_move = std::make_unique<ControlledMovement>(m_mesh, m_displayedMesh.get());
}

void RedShell::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		m_move->Tick();
	}
}

void RedShell::HitByPlayer(Player * _player)
{
}

void RedShell::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);
	
	m_mesh->SetWorld(_player->GetWorld());
	m_mesh->AddPos(_player->GetWorld().Right() * 1);
	m_mesh->SetOri(m_player->GetOri());
	m_mesh->UpdateWorld();
	Vector3 normVel = _player->getVelocity();
	normVel.Normalize();
	m_mesh->setVelocity(normVel * (_altUse? -m_altUseSpeed : 1));

	if (!_altUse)
	{
		m_move->SetWaypoint(_player->GetWaypoint());
		m_ai = std::make_unique<MoveAI>(m_mesh, m_move.get());
		Locator::getAIScheduler()->AddAI(m_ai.get());
	}

}
