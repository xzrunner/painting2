#pragma once

#include <SM_Matrix2D.h>
#include <SM_Rect.h>

#include <functional>

namespace n0 { class CompAsset; }
namespace pt0 { class Color; }

namespace pt2
{

class Textbox;

class Callback
{
public:
	struct Funs
	{
		std::function<void(const std::string&, const Textbox&,
			const sm::Matrix2D&, const pt0::Color&, const pt0::Color&)> draw_text;
		std::function<sm::vec2(const std::string&, const Textbox&)> calc_label_size;

		std::function<sm::rect(const n0::CompAsset&)> get_bounding;

		std::function<const float*(size_t, const sm::irect&, int&)>  query_cached_tex_quad;
		std::function<void(size_t, int, int, const sm::irect& rect)> add_cache_symbol;
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void DrawText(const std::string& text, const Textbox& style,
		const sm::Matrix2D& mat, const pt0::Color& mul_col, const pt0::Color& add_col);
	static sm::vec2 CalcLabelSize(const std::string& text, const Textbox& style);

	static sm::rect GetBounding(const n0::CompAsset& casset);

	static const float* QueryCachedTexQuad(size_t tex_id, const sm::irect& rect, int& out_tex_id);
	static void AddCacheSymbol(size_t tex_id, int tex_w, int tex_h, const sm::irect& rect);

}; // Callback

}