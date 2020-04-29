#pragma once

#include <memory>

namespace ur { class Device; class Context; }

namespace pt2
{

class DebugDraw
{
public:
	// 2 1
	// 3 4
	// 0 for all
	static void Draw(const ur::Device& dev, ur::Context& ctx,
        int tex_id, int pos);

}; // DebugDraw

}