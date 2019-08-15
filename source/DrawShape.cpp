#include "painting2/DrawShape.h"

#include <tessellation/Painter.h>
#include <primitive/Path.h>
#include <geoshape/config.h>

#include <geoshape/Point2D.h>
#include <geoshape/Rect.h>
#include <geoshape/Circle.h>
#include <geoshape/Polyline2D.h>
#include <geoshape/Bezier.h>
#include <geoshape/Polygon2D.h>

namespace pt2
{

void DrawShape::Draw(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale)
{
    auto type = shape.get_type();
	if (type == rttr::type::get<gs::Point2D>())
	{
		auto& p = static_cast<const gs::Point2D&>(shape);
		pt.AddCircleFilled(p.GetPos(), gs::NODE_QUERY_RADIUS * cam_scale, color);
	}
	else if (type == rttr::type::get<gs::Rect>())
	{
		auto& r = static_cast<const gs::Rect&>(shape).GetRect();
		prim::Path p;
		p.Rect({ r.xmin, r.ymin }, r.Width(), r.Height());
		auto& vertices = p.GetCurrPath();
		pt.AddPolyline(vertices.data(), vertices.size(), color, cam_scale);
	}
	else if (type == rttr::type::get<gs::Circle>())
	{
		auto& c = static_cast<const gs::Circle&>(shape);
		pt.AddCircle(c.GetCenter(), c.GetRadius(), color, cam_scale, static_cast<uint32_t>(c.GetRadius() * 0.5f));
	}
	else if (type == rttr::type::get<gs::Polyline2D>())
	{
		auto& p = static_cast<const gs::Polyline2D&>(shape).GetVertices();
		pt.AddPolyline(p.data(), p.size(), color, cam_scale);
	}
	else if (type == rttr::type::get<gs::Bezier>())
	{
		auto& p = static_cast<const gs::Bezier&>(shape).GetVertices();
		pt.AddPolyline(p.data(), p.size(), color, cam_scale);
	}
	else if (type == rttr::type::get<gs::Polygon2D>())
	{
		auto& p = static_cast<const gs::Polygon2D&>(shape).GetVertices();
		pt.AddPolygon(p.data(), p.size(), color, cam_scale);
	}
}

}