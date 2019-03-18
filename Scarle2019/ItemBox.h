#pragma once

#include "PhysModel.h"
#include "SimpleMath.h"
#include <iostream>

class ItemBox : public PhysModel {
public:
	ItemBox(Vector3& position, Vector3& rotation);
	~ItemBox() = default;

private:
};