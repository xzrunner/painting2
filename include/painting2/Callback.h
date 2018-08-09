#pragma once

#include <SM_Matrix2D.h>
#include <SM_Rect.h>

#include <functional>

namespace n0 { class CompAsset; }

namespace pt2
{

class Text;
class RenderColorCommon;

class Callback
{
public:
	struct Funs
	{
		std::function<void(const Text&, const sm::Matrix2D&, const RenderColorCommon&)> draw_text;
		std::function<sm::vec2(const std::string&, const Textbox&)> calc_label_size;

		std::function<sm::rect(const n0::CompAsset&)> get_bounding;
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void DrawText(const Text& text, const sm::Matrix2D& mat, 
		const RenderColorCommon& col);
	static sm::vec2 CalcLabelSize(const std::string& text, const Textbox& style);

	static sm::rect GetBounding(const n0::CompAsset& casset);

}; // Callback

}