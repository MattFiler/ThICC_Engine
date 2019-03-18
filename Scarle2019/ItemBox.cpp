#include "pch.h"
#include "ItemBox.h"


//Todo: replace "Default Item" with item box model
ItemBox::ItemBox(Vector3& position, Vector3& rotation) : PhysModel("Default Item") {
	SetPos(position);
	SetRotation(rotation);

	std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}