#pragma once

#include <cstdint>

namespace tess { class Painter; }
namespace gs { class Shape; }

namespace pt2
{

class DrawShape
{
public:
	static void Draw(tess::Painter& pt, const gs::Shape& shape, uint32_t color, float cam_scale);

}; // DrawShape

}