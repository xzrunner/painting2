#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>
#include <SM_Matrix.h>
#include <cu/cu_macro.h>

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
	auto& GetPainter() { return *m_painter; }
	void SetPainterMat(const sm::mat4& pt_mat);
	void FlushPainter() const;

	void DrawShape(const gs::Shape& shape, const sm::mat4& mat);

	static void DrawTexture(const Texture& tex, const sm::rect& pos,
		const sm::Matrix2D& mat);
	static void DrawTexture(const Texture& tex, const sm::mat4& mat);
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

private:
	// display list
	std::unique_ptr<tess::Painter> m_painter = nullptr;
	sm::mat4 m_pt_mat;

	CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}