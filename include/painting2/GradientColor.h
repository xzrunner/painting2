#pragma once

#include "painting2/Color.h"

#include <vector>

namespace pt2
{

class Color;

class GradientColor
{
public:
	struct Item
	{
		Item() : pos(-1) {}

		Color col;
		float pos;
	};

	Item begin, mid, end;

}; // GradientColor

}