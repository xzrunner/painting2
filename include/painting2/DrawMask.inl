#pragma once

namespace pt2
{

template<typename Type, typename Params>
void DrawMask::Draw(const Type& base, const Type& mask, const Params& params,
	                std::function<void(const Type& data, const Params& params)> draw_cb)
{

}

template<typename Type, typename Params>
void DrawMask::DrawBaseToRT(pt2::RenderTarget* rt, const Type& base, const Params& params,
	                        std::function<void(const Type& data, const Params& params)> draw_cb)
{

}

template<typename Type, typename Params>
void DrawMask::DrawMaskToRT(pt2::RenderTarget* rt, const Type& mask, const Params& params,
	                        std::function<void(const Type& data, const Params& params)> draw_cb)
{

}

template<typename Type, typename Params>
void DrawMask::DrawMaskFromRT(pt2::RenderTarget* rt_base, pt2::RenderTarget* rt_mask, 
	                          const Type& mask, const S2_MAT& mt)
{

}

}