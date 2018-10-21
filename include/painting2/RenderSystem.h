#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>
#include <SM_Matrix.h>

#include <string>
#include <memory>

namespace tess { class Painter; }
namespace gs { class Shape; }

namespace pt2
{

class Texture;
class Text;
class Textbox;
class RenderColorCommon;
class RenderColorMap;
class Color;
class Shader;

class RenderSystem
{
public:
	static void DrawPainter(const tess::Painter& pt, const sm::mat4& mat);

	static void DrawShape(tess::Painter& pt, const gs::Shape& shape, uint32_t color);

	static void DrawTexQuad(const float* positions, const float* texcoords, int texid, uint32_t color);

	static void DrawTexture(const Texture& tex, const sm::rect& pos,
		const sm::Matrix2D& mat);
	static void DrawTexture(const std::shared_ptr<Shader>& shader,
		const sm::mat4& mat);

	static void DrawColor(const std::shared_ptr<Shader>& shader,
		const sm::mat4& mat);

	static void DrawText(const std::string& text, const Textbox& style,
		const sm::Matrix2D& mat, const Color& mul_col, const Color& add_col);

	static void SetColor(const RenderColorCommon& col);
	static void SetColorMap(const RenderColorMap& col);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat,
		float* vertices);

}; // RenderSystem

}