#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>

#include <string>

namespace pt2
{

class Texture;
class Text;
class Textbox;
class RenderColorCommon;
class RenderColorMap;
class Color;

class RenderSystem
{
public:
	static void DrawTexture(const Texture& tex, const sm::rect& pos, 
		const sm::Matrix2D& mat);

	static void DrawText(const std::string& text, const Textbox& style, 
		const sm::Matrix2D& mat, const Color& mul_col, const Color& add_col);

	static void SetColor(const RenderColorCommon& col);
	static void SetColorMap(const RenderColorMap& col);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat, 
		float* vertices);

}; // RenderSystem

}