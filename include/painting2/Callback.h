#pragma once

#include <SM_Matrix2D.h>

namespace pt2
{

class Text;
class RenderColorCommon;

class Callback
{
public:
	struct Funs
	{
		void(*draw_text)(const Text& text, const sm::Matrix2D& mat, const RenderColorCommon& col);
	};

	static void RegisterCallback(const Funs& funs);

	//////////////////////////////////////////////////////////////////////////

	static void DrawText(const Text& text, const sm::Matrix2D& mat, 
		const RenderColorCommon& col);

}; // Callback

}