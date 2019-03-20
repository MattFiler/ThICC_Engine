#include "pch.h"
#include "Item.h"
#include <fstream>

Item::Item(const std::string& item_type) 
{
	//Set model name
	m_mesh = new TrackMagnet(item_type);
}

void Item::Tick()
{
	if (m_mesh)
	{
		m_mesh->Tick();
	}
}
