#pragma once

#include <SM_Matrix2D.h>
#include <SM_Rect.h>

#include <functional>

namespace n0 { class CompAsset; }

namespace pt2
{

class Textbox;
class Color;

class Callback
{
public:
	struct Funs
	{
		std::function<void(const std::string&, const Textbox&,
			const sm::Matrix2D&, const Color&, const Color&)> draw_text;
		std::function<sm::vec2(const std::string&, const Textbox&)> calc_label_size;

		std::function<sm::rect(const n0::CompAsset&)> get_bounding;

		std::function<const float*(size_t, const sm::irect&, int&)>  query_cached_tex_quad;
		std::function<void(size_t, int, int, const sm::irect& rect)> add_cache_symbol;
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void DrawText(const std::string& text, const Textbox& style,
		const sm::Matrix2D& mat, const Color& mul_col, const Color& add_col);
	static sm::vec2 CalcLabelSize(const std::string& text, const Textbox& style);

	static sm::rect GetBounding(const n0::CompAsset& casset);

	static const float* QueryCachedTexQuad(size_t tex_id, const sm::irect& rect, int& out_tex_id);
	static void AddCacheSymbol(size_t tex_id, int tex_w, int tex_h, const sm::irect& rect);

}; // Callback

}