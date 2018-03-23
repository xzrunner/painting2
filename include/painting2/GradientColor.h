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

		bool operator == (const Item& item) const {
			return col == item.col && pos == item.pos;
		}
	};

	std::vector<Item> items;

	int angle;

	GradientColor() : angle(0) {
		items.resize(1);
	}

	bool operator == (const GradientColor& col) const;

}; // GradientColor

}