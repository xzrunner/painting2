#pragma once

#include <cstdint>

namespace tess { class Painter; }
namespace gs { class Shape2D; }

namespace pt2
{

class DrawShape
{
public:
	static void Draw(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale);

}; // DrawShape

}