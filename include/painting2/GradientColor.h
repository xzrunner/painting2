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
		Item() : pos(0) {}
		Item(float p) : pos(p) {}

		Color col;
		float pos;

		bool operator == (const Item& item) const {
			return col == item.col && pos == item.pos;
		}
	};

	std::vector<Item> items;

	float angle;

	GradientColor() : angle(0) {
		items.resize(1);
	}

	bool operator == (const GradientColor& col) const;

}; // GradientColor

}