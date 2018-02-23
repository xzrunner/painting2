#pragma once

#include <functional>

namespace pt2
{

class RenderTarget;

class DrawMask
{
public:
	template<typename Type, typename Params>
	static void Draw(const Type& base, const Type& mask, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

private:
	template<typename Type, typename Params>
	static void DrawBaseToRT(RenderTarget* rt, const Type& base, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

	template<typename Type, typename Params>
	static void DrawMaskToRT(RenderTarget* rt, const Type& mask, const Params& params,
		std::function<void(const Type& data, const Params& params)> draw_cb);

	template<typename Type, typename Params>
	static void DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, 
		const Type& mask, const Params& params);

}; // DrawMask

}

#include "painting2/DrawMask.inl"