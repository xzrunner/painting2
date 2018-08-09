#pragma once

#include <SM_Vector.h>

#include <vector>

namespace pt2
{

class BezierShape
{
public:
	auto& GetVertices() const { return m_vertices; }

	void SetCtrlPos(const sm::vec2& v0, const sm::vec2& v1,
		const sm::vec2& v2, const sm::vec2& v3);

private:
	void UpdatePolyline();

	sm::vec2 PointOnCubicBezier(float t);

public:
	static const int CTRL_NODE_COUNT = 4;

private:
	// [0] start, [1] ctrl1, [2] ctrl2, [3] end
	sm::vec2 m_control_nodes[CTRL_NODE_COUNT];

	std::vector<sm::vec2> m_vertices;

}; // BezierShape

}