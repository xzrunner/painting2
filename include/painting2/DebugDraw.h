#pragma once

#include <memory>

namespace ur2 { class Device; class Context; }

namespace pt2
{

class DebugDraw
{
public:
	// 2 1
	// 3 4
	// 0 for all
	static void Draw(const ur2::Device& dev, ur2::Context& ctx,
        int tex_id, int pos);

}; // DebugDraw

}