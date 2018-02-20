#include "painting2/Callback.h"

namespace pt2
{

static Callback::Funs FUNS;

void Callback::RegisterCallback(const Callback::Funs& funs)
{
	FUNS = funs;
}

void Callback::DrawText(const Text& text, const sm::Matrix2D& mat)
{
	FUNS.draw_text(text, mat);
}

}