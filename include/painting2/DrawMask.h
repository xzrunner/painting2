#pragma once

#include <functional>

namespace pt2
{

class DrawMask
{
public:
	template<typename Type, typename Params>
	static void Draw(const Type& base, const Type& mask, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

private:
	template<typename Type, typename Params>
	static void DrawBaseToRT(pt2::RenderTarget* rt, const Type& base, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

	template<typename Type, typename Params>
	static void DrawMaskToRT(pt2::RenderTarget* rt, const Type& mask, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

	template<typename Type, typename Params>
	static void DrawMaskFromRT(pt2::RenderTarget* rt_base, pt2::RenderTarget* rt_mask, const Type& mask, const S2_MAT& mt);

}; // DrawMask

}

#include "painting2/DrawMask.inl"