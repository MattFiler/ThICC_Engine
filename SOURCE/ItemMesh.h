#pragma once
#include "TrackMagnet.h"
#include "AnimationController.h"
#include <memory>
struct ItemMesh
{
	std::unique_ptr<TrackMagnet> m_mesh = nullptr;
	std::unique_ptr<AnimationController> m_displayedMesh = nullptr;
};