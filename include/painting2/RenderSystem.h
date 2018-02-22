#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>

namespace pt2
{

class Texture;
class Text;
class RenderColorCommon;
class RenderColorMap;

class RenderSystem
{
public:
	static void DrawTexture(const Texture& tex, const sm::rect& pos, 
		const sm::Matrix2D& mat);

	static void DrawText(const Text& text, const sm::Matrix2D& mat);

	static void DrawMask();

	static void SetColor(const RenderColorCommon& col);
	static void SetColorMap(const RenderColorMap& col);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat, 
		float* vertices);

}; // RenderSystem

}