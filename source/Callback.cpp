#include "painting2/Callback.h"

namespace pt2
{

static Callback::Funs FUNS;

void Callback::RegisterCallback(const Callback::Funs& funs)
{
	FUNS = funs;
}

void Callback::DrawText(const std::string& text, const Textbox& style, const sm::Matrix2D& mat,
	                    const Color& mul_col, const Color& add_col)
{
	FUNS.draw_text(text, style, mat, mul_col, add_col);
}

sm::vec2 Callback::CalcLabelSize(const std::string& text, const Textbox& style)
{
	return FUNS.calc_label_size(text, style);
}

sm::rect Callback::GetBounding(const n0::CompAsset& casset)
{
	return FUNS.get_bounding(casset);
}

const float* Callback::QueryCachedTexQuad(size_t tex_id, const sm::irect& rect, int& out_tex_id)
{
	return FUNS.query_cached_tex_quad(tex_id, rect, out_tex_id);
}

void Callback::AddCacheSymbol(size_t tex_id, int tex_w, int tex_h, const sm::irect& rect)
{
	FUNS.add_cache_symbol(tex_id, tex_w, tex_h, rect);
}

}