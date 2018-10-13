#include "painting2/ShapeRenderer.h"
#include "painting2/RenderSystem.h"

#include <geoshape/Shape2D.h>
#include <tessellation/Painter.h>

namespace pt2
{

void ShapeRenderer::Draw(const gs::Shape& shape, const sm::mat4& mat)
{
	tess::Painter pt;

	auto& s2 = static_cast<const gs::Shape2D&>(shape);
	s2.Draw([&](const sm::vec2* vertices, size_t n, bool close, bool filling) {
		if (close)
		{
			if (filling) {
				pt.AddPolygonFilled(vertices, n, 0xff0000ff);
			} else {
				pt.AddPolygon(vertices, n, 0xff0000ff);
			}
		}
		else
		{
			pt.AddPolyline(vertices, n, 0xff00ffff);
		}
	});

	RenderSystem::DrawPrimitive(pt, mat);
}

}