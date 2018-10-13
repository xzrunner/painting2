#pragma once

#include <SM_Matrix.h>

namespace gs { class Shape; }

namespace pt2
{

class ShapeRenderer
{
public:
	static void Draw(const gs::Shape& shape,
		const sm::mat4& mat);

}; // ShapeRenderer

}