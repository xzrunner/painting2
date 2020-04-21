#pragma once

#include <SM_Rect.h>
#include <SM_Matrix2D.h>
#include <SM_Matrix.h>
#include <unirender2/typedef.h>

#include <string>
#include <memory>

namespace tess { class Painter; }
namespace gs { class Shape2D; }
namespace pt0 { class Color; }
namespace ur2 {
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
	static void DrawPainter(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
        const tess::Painter& pt, const sm::mat4& mat = sm::mat4());

	static void DrawShape(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale = 1.0f);

	static void DrawTexQuad(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
        const float* positions, const float* texcoords, int tex_id, uint32_t color);

	static void DrawTexture(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
        const ur2::TexturePtr& tex, const sm::rect& pos, const sm::Matrix2D& mat, bool use_dtex = true);
    static void DrawTexture(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
        int tex_w, int tex_h, int tex_id, const sm::rect& pos, const sm::Matrix2D& mat, bool use_dtex = true);
	static void DrawTexture(const ur2::Device& dev, ur2::Context& ctx,
        const std::shared_ptr<ur2::ShaderProgram>& shader, const sm::mat4& mat);

	static void DrawColor(const ur2::Device& dev, ur2::Context& ctx,
        const std::shared_ptr<ur2::ShaderProgram>& shader, const sm::mat4& mat);

	static void DrawText(ur2::Context& ctx, const std::string& text, const Textbox& style,
		const sm::Matrix2D& mat, const pt0::Color& mul_col, const pt0::Color& add_col);

	static void SetColor(const RenderColorCommon& col);
	static void SetColorMap(const RenderColorMap& col);

private:
	static bool CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat,
		float* vertices);

}; // RenderSystem

}