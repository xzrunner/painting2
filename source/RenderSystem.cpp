#include "painting2/RenderSystem.h"
#include "painting2/Texture.h"
#include "painting2/RenderColorCommon.h"
#include "painting2/RenderColorMap.h"
#include "painting2/Callback.h"
#include "painting2/DrawMask.h"
#include "painting2/DrawShape.h"

#include <tessellation/Painter.h>
#include <unirender2/Texture.h>
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/SpriteRenderer.h>
#include <renderpipeline/ExternRenderer.h>

namespace pt2
{

void RenderSystem::DrawPainter(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
                               const tess::Painter& pt, const sm::mat4& mat)
{
	if (pt.IsEmpty()) {
		return;
	}
	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawPainter(ctx, rs, pt, mat);
}

void RenderSystem::DrawShape(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale)
{
	DrawShape::Draw(pt, shape, color, cam_scale);
}

void RenderSystem::DrawTexQuad(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
                               const float* positions, const float* texcoords,
                               const ur2::TexturePtr& tex, uint32_t color)
{
	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, positions, texcoords, tex, color);
}

void RenderSystem::DrawTexture(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
                               const ur2::TexturePtr& tex, const sm::rect& pos,
                               const sm::Matrix2D& mat, bool use_dtex)
{
    DrawTexture(dev, ctx, rs, tex->GetWidth(), tex->GetHeight(), tex, pos, mat, use_dtex);
}

void RenderSystem::DrawTexture(const ur2::Device& dev, ur2::Context& ctx, const ur2::RenderState& rs,
                               int tex_w, int tex_h, const ur2::TexturePtr& tex, const sm::rect& pos, const sm::Matrix2D& mat, bool use_dtex)
{
	float vertices[8];
	CalcVertices(pos, mat, vertices);

	auto draw_without_dtex = [&](std::shared_ptr<rp::IRenderer>& rd, const float* vertices, const ur2::TexturePtr& tex)
	{
		float txmin, txmax, tymin, tymax;
		txmin = tymin = 0;
		txmax = tymax = 1;
		float texcoords[8] = {
			txmin, tymin,
			txmax, tymin,
			txmax, tymax,
			txmin, tymax,
		};
		std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, vertices, texcoords, tex, 0xffffffff);
	};

	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
	// query from dtex
    // todo use TexturePtr
	if (use_dtex)
	{
		sm::irect qr(0, 0, tex_w, tex_h);
        ur2::TexturePtr cached_tex = nullptr;
		auto cached_texcoords = Callback::QueryCachedTexQuad(tex->GetTexID(), qr, cached_tex);
		if (cached_texcoords) {
			std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, vertices, cached_texcoords, cached_tex, 0xffffffff);
		} else {
			draw_without_dtex(rd, vertices, tex);
			Callback::AddCacheSymbol(tex, qr);
		}
	}
	else
	{
		draw_without_dtex(rd, vertices, tex);
	}
}

void RenderSystem::DrawTexture(const ur2::Device& dev, ur2::Context& ctx,
                               const std::shared_ptr<ur2::ShaderProgram>& shader, const sm::mat4& mat)
{
	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::EXTERN);
	std::static_pointer_cast<rp::ExternRenderer>(rd)->DrawTexSpr(ctx, shader, mat);
}

void RenderSystem::DrawColor(const ur2::Device& dev, ur2::Context& ctx,
                             const std::shared_ptr<ur2::ShaderProgram>& shader, const sm::mat4& mat)
{
	auto sr = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::EXTERN);
	std::static_pointer_cast<rp::ExternRenderer>(sr)->DrawNoTexSpr(ctx, shader, mat);
}

void RenderSystem::DrawText(ur2::Context& ctx, const std::string& text, const Textbox& style,
		                    const sm::Matrix2D& mat, const pt0::Color& mul_col, const pt0::Color& add_col)
{
	Callback::DrawText(ctx, text, style, mat, mul_col, add_col);
}

void RenderSystem::SetColor(const RenderColorCommon& col)
{
	//auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	//// todo: other shader
	//auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	//shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());
}

void RenderSystem::SetColorMap(const RenderColorMap& col)
{
	//auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	//// todo: other shader
	//auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	//shader->SetColorMap(col.rmap.ToABGR(), col.gmap.ToABGR(), col.bmap.ToABGR());
}

bool RenderSystem::CalcVertices(const sm::rect& pos, const sm::Matrix2D& mat, float* vertices)
{
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax = -FLT_MAX, ymax = -FLT_MAX;

	const float* mt = mat.x;

	// Vertices are ordered for use with triangle strips:
	// 3----2
	// |  / |
	// | /  |
	// 0----1

	float x, y;

	float* ptr_dst = &vertices[0];

	x = (pos.xmin * mt[0] + pos.ymin * mt[2]) + mt[4];
	y = (pos.xmin * mt[1] + pos.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmax * mt[0] + pos.ymin * mt[2]) + mt[4];
	y = (pos.xmax * mt[1] + pos.ymin * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmax * mt[0] + pos.ymax * mt[2]) + mt[4];
	y = (pos.xmax * mt[1] + pos.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	x = (pos.xmin * mt[0] + pos.ymax * mt[2]) + mt[4];
	y = (pos.xmin * mt[1] + pos.ymax * mt[3]) + mt[5];
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	*ptr_dst++ = x;
	*ptr_dst++ = y;

	//if (rp.IsViewRegionValid()) {
	//	const sm::rect& vr = rp.GetViewRegion();
	//	if (xmax <= vr.xmin || xmin >= vr.xmax ||
	//		ymax <= vr.ymin || ymin >= vr.ymax) {
	//		return false;
	//	}
	//}

	return true;
}

}