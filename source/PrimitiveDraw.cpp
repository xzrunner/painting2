#include "painting2/PrimitiveDraw.h"
#include "painting2/config.h"

#ifndef PT2_DISABLE_RVG
#include <rvg.h>
#endif // PT2_DISABLE_RVG
#include <SM_Calc.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shape2Shader.h>
#include <shaderlab/Shape3Shader.h>
#ifndef PT2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // PT2_DISABLE_DEFERRED
#include <painting2/Color.h>
#include <painting2/CameraMgr.h>

#include <assert.h>

namespace pt2
{

void PrimitiveDraw::Init()
{
#ifndef PT2_DISABLE_RVG
	rvg_style_init();
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::SetColor(const pt2::Color& color)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(mgr->GetShader(sl::SHAPE2));
		shader->SetColor(color.ToABGR());
	} else {
		sl::Shape3Shader* shader = static_cast<sl::Shape3Shader*>(mgr->GetShader(sl::SHAPE3));
		shader->SetColor(color.ToABGR());
	}
}

void PrimitiveDraw::PointSize(float size)
{
#ifndef PT2_DISABLE_RVG
	rvg_point_size(size);
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::LineWidth(float width)
{
#ifndef PT2_DISABLE_RVG
	rvg_line_width(width);
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Point(cooking::DisplayList* dlist, const sm::vec2& vertex)
{
#ifndef PT2_DISABLE_RVG
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_point(vertex.x, vertex.y);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_point3(vertex.x, vertex.y, 0);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Points(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices)
{
#ifndef PT2_DISABLE_RVG
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point(vertices[i].x, vertices[i].y);
		}
	} else {
		SetShader(dlist, sl::SHAPE3);
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			rvg_point3(vertices[i].x, vertices[i].y, 0);
		}
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Line(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1)
{
#ifndef PT2_DISABLE_RVG
	rvg_line_style(LS_DEFAULT);

	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::DotLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1)
{
#ifndef PT2_DISABLE_RVG
	rvg_line_style(LS_DOT);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::DashLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1)
{
#ifndef PT2_DISABLE_RVG
	rvg_line_style(LS_DASH);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::DotDashLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1)
{
#ifndef PT2_DISABLE_RVG
	rvg_line_style(LS_DOT_DASH);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_line(p0.x, p0.y, p1.x, p1.y);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_line3(p0.x, p0.y, 0, p1.x, p1.y, 0);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Lines(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices)
{
#ifndef PT2_DISABLE_RVG
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_lines(&vertices[0].x, vertices.size());
	} else {
		SetShader(dlist, sl::SHAPE3);
		CU_VEC<sm::vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(sm::vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_lines(&vertices3[0].x, vertices3.size());
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Polyline(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices, bool loop)
{
#ifndef PT2_DISABLE_RVG
	if (vertices.size() < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_polyline(&vertices[0].x, vertices.size(), loop);
	} else {
		SetShader(dlist, sl::SHAPE3);
		CU_VEC<sm::vec3> vertices3;
		for (int i = 0, n = vertices.size(); i < n; ++i) {
			vertices3.push_back(sm::vec3(vertices[i].x, vertices[i].y, 0));
		}
		rvg_polyline3(&vertices3[0].x, vertices3.size(), loop);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Polyline(cooking::DisplayList* dlist, const float* vertices, int count, bool loop)
{
#ifndef PT2_DISABLE_RVG
	if (count < 2) {
		return;
	}

	rvg_line_style(LS_DEFAULT);
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_polyline(vertices, count, loop);
	} else {
		SetShader(dlist, sl::SHAPE3);
		CU_VEC<sm::vec3> vertices3;
		vertices3.reserve(count);
		for (int i = 0; i < count; ++i) {
			vertices3.push_back(sm::vec3(vertices[i * 2], vertices[i * 2 + 1], 0));
		}
		rvg_polyline3(&vertices3[0].x, vertices3.size(), loop);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Triangles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles)
{
#ifndef PT2_DISABLE_RVG
	if (triangles.size() < 3) return;

	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_triangles(&triangles[0].x, triangles.size());
	} else {
		SetShader(dlist, sl::SHAPE3);
		CU_VEC<sm::vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(sm::vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangles3(&triangles3[0].x, triangles3.size());
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Triangles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles, const CU_VEC<pt2::Color>& colors)
{
	// todo
}

void PrimitiveDraw::TriangleStrip(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles)
{
#ifndef PT2_DISABLE_RVG
	if (triangles.size() < 3) return;

	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_triangle_strip(&triangles[0].x, triangles.size());
	} else {
		SetShader(dlist, sl::SHAPE3);
		CU_VEC<sm::vec3> triangles3;
		for (int i = 0, n = triangles.size(); i < n; ++i) {
			triangles3.push_back(sm::vec3(triangles[i].x, triangles[i].y, 0));
		}
		rvg_triangle_strip3(&triangles3[0].x, triangles3.size());
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Rect(cooking::DisplayList* dlist, const sm::vec2& center, float hw, float hh, bool filling)
{
	Rect(dlist, center - sm::vec2(hw, hh), center + sm::vec2(hw, hh), filling);
}

void PrimitiveDraw::Rect(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1, bool filling)
{
#ifndef PT2_DISABLE_RVG
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_rect(p0.x, p0.y, p1.x, p1.y, filling);
		sl::ShaderMgr::Instance()->GetShader()->Commit();
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_rect3_on_z(p0.x, p0.y, p1.x, p1.y, 0, filling);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Rect(cooking::DisplayList* dlist, const sm::rect& r, bool filling)
{
	Rect(dlist, sm::vec2(r.xmin, r.ymin), sm::vec2(r.xmax, r.ymax), filling);
}

void PrimitiveDraw::Circle(cooking::DisplayList* dlist, const sm::vec2& center, float radius, bool filling, int segments)
{
#ifndef PT2_DISABLE_RVG
	if (pt2::CameraMgr::Instance()->IsType(pt2::CameraMgr::ORTHO)) {
		SetShader(dlist, sl::SHAPE2);
		rvg_circle(center.x, center.y, radius, filling, segments);
	} else {
		SetShader(dlist, sl::SHAPE3);
		rvg_circle3(center.x, center.y, 0, radius, filling, segments);
	}
#endif // PT2_DISABLE_RVG
}

void PrimitiveDraw::Circles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& circles, float radius, bool filling, int segments)
{
	for (int i = 0, n = circles.size(); i < n; ++i) {
		Circle(dlist, circles[i], radius, filling, segments);
	}
}

void PrimitiveDraw::Cross(cooking::DisplayList* dlist, const sm::vec2& center, float edge)
{
	Cross(dlist, center, edge, edge);
}

void PrimitiveDraw::Cross(cooking::DisplayList* dlist, const sm::vec2& center, float xedge, float yedge)
{
	sm::vec2 s = center, e = center;
	s.x -= xedge;
	e.x += xedge;
	Line(dlist, s, e);

	s  = e = center;
	s.y -= yedge;
	e.y += yedge;
	Line(dlist, s, e);
}

void PrimitiveDraw::Arrow(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1, float len)
{
	sm::vec2 seg = p0 - p1;
	seg.Normalize();
	seg *= len;

	sm::vec2 left, right;
	left = p1 + sm::rotate_vector(seg, - SM_PI / 6);
	right = p1 + sm::rotate_vector(seg, SM_PI / 6);
	Line(dlist, p1, left);
	Line(dlist, p1, right);
}

void PrimitiveDraw::SetShader(cooking::DisplayList* dlist, int shader_type)
{
#ifdef PT2_DISABLE_DEFERRED
	sl::ShaderMgr::Instance()->SetShader(static_cast<sl::ShaderType>(shader_type));
#else
	assert(dlist);
	cooking::change_shader(dlist, shader_type);
#endif // PT2_DISABLE_DEFERRED
}

}