#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>
#include <SM_Matrix.h>
#include <unirender/typedef.h>

#include <string>
#include <memory>

namespace tess { class Painter; }
namespace gs { class Shape2D; }
namespace pt0 { class Color; }
namespace ur {
    class Device;
    class Context;
    class Texture;
    class ShaderProgram;
    struct RenderState;
}

namespace pt2
{

class Text;
class Textbox;
class RenderColorCommon;
class RenderColorMap;

class RenderSystem
{
public:
	static void DrawPainter(const ur::Device& dev, ur::Context& ctx, const ur::RenderState& rs,
        const tess::Painter& pt, const sm::mat4& mat = sm::mat4());

	static void DrawShape(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale = 1.0f);

	static void DrawTexQuad(const ur::Device& dev, ur::Context& ctx, const ur::RenderState& rs,
        const float* positions, const float* texcoords, const ur::TexturePtr& tex, uint32_t color);

	static void DrawTexture(const ur::Device& dev, ur::Context& ctx, const ur::RenderState& rs,
        const ur::TexturePtr& tex, const sm::rect& pos, const sm::Matrix2D& mat, bool use_dtex = true);
    static void DrawTexture(const ur::Device& dev, ur::Context& ctx, const ur::RenderState& rs,
        int tex_w, int tex_h, const ur::TexturePtr& tex, const sm::rect& pos, const sm::Matrix2D& mat, bool use_dtex = true);
	static void DrawTexture(const ur::Device& dev, ur::Context& ctx,
        const std::shared_ptr<ur::ShaderProgram>& shader, const sm::mat4& mat);

	static void DrawColor(const ur::Device& dev, ur::Context& ctx,
        const std::shared_ptr<ur::ShaderProgram>& shader, const sm::mat4& mat);

	static void DrawText(ur::Context& ctx, const std::string& text, const Textbox& style,
		const sm::Matrix2D& mat, const pt0::Color& mul_col, const pt0::Color& add_col);

	static void SetColor(const RenderColorCommon& col);
	static void SetColorMap(const RenderColorMap& col);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat,
		float* vertices);

}; // RenderSystem

}