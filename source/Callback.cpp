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

}