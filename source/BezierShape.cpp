#include "painting2/BezierShape.h"

#include <SM_Calc.h>

namespace pt2
{

void BezierShape::SetCtrlPos(const sm::vec2& v0, const sm::vec2& v1,
	                         const sm::vec2& v2, const sm::vec2& v3)
{
	if (m_control_nodes[0] == v0 &&
		m_control_nodes[1] == v1 &&
		m_control_nodes[2] == v2 &&
		m_control_nodes[3] == v3) {
		return;
	}

	m_control_nodes[0] = v0;
	m_control_nodes[1] = v1;
	m_control_nodes[2] = v2;
	m_control_nodes[3] = v3;

	UpdatePolyline();
}

void BezierShape::UpdatePolyline()
{
	const int num = std::max(20, (int)(sm::dis_pos_to_pos(m_control_nodes[0], m_control_nodes[3]) / 10));
	float dt = 1.0f / (num - 1);
	m_vertices.resize(num);
	for (int i = 0; i < num; ++i) {
		m_vertices[i] = PointOnCubicBezier(i * dt);
	}
}

sm::vec2 BezierShape::PointOnCubicBezier(float t)
{
	float ax, bx, cx;
	float ay, by, cy;
	float squared, cubed;
	sm::vec2 result;

	cx = 3.0f * (m_control_nodes[1].x - m_control_nodes[0].x);
	bx = 3.0f * (m_control_nodes[2].x - m_control_nodes[1].x) - cx;
	ax = m_control_nodes[3].x - m_control_nodes[0].x - cx - bx;

	cy = 3.0f * (m_control_nodes[1].y - m_control_nodes[0].y);
	by = 3.0f * (m_control_nodes[2].y - m_control_nodes[1].y) - cy;
	ay = m_control_nodes[3].y - m_control_nodes[0].y - cy - by;

	squared = t * t;
	cubed = squared * t;

	result.x = (ax * cubed) + (bx * squared) + (cx * t) + m_control_nodes[0].x;
	result.y = (ay * cubed) + (by * squared) + (cy * t) + m_control_nodes[0].y;

	return result;
}

}