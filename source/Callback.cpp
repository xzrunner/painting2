#include "painting2/Callback.h"

namespace pt2
{

static Callback::Funs FUNS;

void Callback::RegisterCallback(const Callback::Funs& funs)
{
	FUNS = funs;
}

void Callback::DrawText(const Text& text, const sm::Matrix2D& mat,
	                    const RenderColorCommon& col)
{
	FUNS.draw_text(text, mat, col);
}

sm::rect Callback::GetBounding(const n0::CompAsset& casset)
{
	return FUNS.get_bounding(casset);
}

}