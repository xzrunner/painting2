#include "painting2/RenderSystem.h"
#include "painting2/Texture.h"
#include "painting2/RenderColorCommon.h"
#include "painting2/RenderColorMap.h"
#include "painting2/Callback.h"
#include "painting2/DrawMask.h"
#include "painting2/DrawShape.h"

#include <unirender/Shader.h>
#include <tessellation/Painter.h>
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/SpriteRenderer.h>
#include <renderpipeline/ExternRenderer.h>

namespace pt2
{

void RenderSystem::DrawPainter(const tess::Painter& pt, const sm::mat4& mat)
{
	if (pt.IsEmpty()) {
		return;
	}
	auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SPRITE);
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawPainter(pt, mat);
}

void RenderSystem::DrawShape(tess::Painter& pt, const gs::Shape2D& shape, uint32_t color, float cam_scale)
{
	DrawShape::Draw(pt, shape, color, cam_scale);
}

void RenderSystem::DrawTexQuad(const float* positions, const float* texcoords, int texid, uint32_t color)
{
	auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SPRITE);
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(positions, texcoords, texid, color);
}

void RenderSystem::DrawTexture(const ur::Texture& tex, const sm::rect& pos,
                               const sm::Matrix2D& mat, bool use_dtex)
{
    DrawTexture(tex.Width(), tex.Height(), tex.TexID(), pos, mat, use_dtex);
}

void RenderSystem::DrawTexture(int tex_w, int tex_h, int tex_id, const sm::rect& pos,
                               const sm::Matrix2D& mat, bool use_dtex)
{
	float vertices[8];
	CalcVertices(pos, mat, vertices);

	auto draw_without_dtex = [](std::shared_ptr<rp::IRenderer>& rd, const float* vertices, int tex_id)
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
		std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(vertices, texcoords, tex_id, 0xffffffff);
	};

	auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SPRITE);
	// query from dtex
	if (use_dtex)
	{
		sm::irect qr(0, 0, tex_w, tex_h);
		int cached_texid;
		auto cached_texcoords = Callback::QueryCachedTexQuad(tex_id, qr, cached_texid);

		if (cached_texcoords) {
			std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(vertices, cached_texcoords, cached_texid, 0xffffffff);
		} else {
			draw_without_dtex(rd, vertices, tex_id);
			Callback::AddCacheSymbol(tex_id, tex_w, tex_h, qr);
		}
	}
	else
	{
		draw_without_dtex(rd, vertices, tex_id);
	}
}

void RenderSystem::DrawTexture(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::EXTERN);
	std::static_pointer_cast<rp::ExternRenderer>(rd)->DrawTexSpr(shader, mat);
}

void RenderSystem::DrawColor(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	auto sr = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::EXTERN);
	std::static_pointer_cast<rp::ExternRenderer>(sr)->DrawNoTexSpr(shader, mat);
}

void RenderSystem::DrawText(const std::string& text, const Textbox& style,
		                    const sm::Matrix2D& mat, const pt0::Color& mul_col, const pt0::Color& add_col)
{
	Callback::DrawText(text, style, mat, mul_col, add_col);
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