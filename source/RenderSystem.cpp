#include "painting2/RenderSystem.h"
#include "painting2/Texture.h"
#include "painting2/RenderColorCommon.h"
#include "painting2/RenderColorMap.h"
#include "painting2/Callback.h"
#include "painting2/DrawMask.h"
#include "painting2/DrawShape.h"

#include <unirender/Shader.h>
#include <tessellation/Painter.h>
#include <rendergraph/RenderMgr.h>
#include <rendergraph/SpriteRenderer.h>
#include <rendergraph/ExternRenderer.h>

namespace pt2
{

void RenderSystem::DrawPainter(const tess::Painter& pt, const sm::mat4& mat)
{
	if (pt.IsEmpty()) {
		return;
	}
	auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE);
	std::static_pointer_cast<rg::SpriteRenderer>(sr)->DrawPainter(pt, mat);
}

void RenderSystem::DrawShape(tess::Painter& pt, const gs::Shape& shape, uint32_t color, float cam_scale)
{
	DrawShape::Draw(pt, shape, color, cam_scale);
}

void RenderSystem::DrawTexQuad(const float* positions, const float* texcoords, int texid, uint32_t color)
{
	auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE);
	std::static_pointer_cast<rg::SpriteRenderer>(sr)->DrawQuad(positions, texcoords, texid, color);
}

void RenderSystem::DrawTexture(const Texture& tex, const sm::rect& pos,
                               const sm::Matrix2D& mat)
{
	float vertices[8];
	CalcVertices(pos, mat, vertices);

	auto draw_without_dtex = [](std::shared_ptr<rg::IRenderer>& sr, const float* vertices, int tex_id)
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
		std::static_pointer_cast<rg::SpriteRenderer>(sr)->DrawQuad(vertices, texcoords, tex_id, 0xffffffff);
	};

	int w = tex.Width();
	int h = tex.Height();
	const bool use_dtex = w < 512 && h < 512;
	auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE);
	// query from dtex
	if (use_dtex)
	{
		sm::irect qr(0, 0, w, h);
		int cached_texid;
		auto cached_texcoords = Callback::QueryCachedTexQuad(tex.TexID(), qr, cached_texid);

		if (cached_texcoords) {
			std::static_pointer_cast<rg::SpriteRenderer>(sr)->DrawQuad(vertices, cached_texcoords, cached_texid, 0xffffffff);
		} else {
			draw_without_dtex(sr, vertices, tex.TexID());
			Callback::AddCacheSymbol(tex.TexID(), w, h, qr);
		}
	}
	else
	{
		draw_without_dtex(sr, vertices, tex.TexID());
	}
}

void RenderSystem::DrawTexture(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::EXTERN);
	std::static_pointer_cast<rg::ExternRenderer>(sr)->DrawTexSpr(shader, mat);
}

void RenderSystem::DrawColor(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::EXTERN);
	std::static_pointer_cast<rg::ExternRenderer>(sr)->DrawNoTexSpr(shader, mat);
}

void RenderSystem::DrawText(const std::string& text, const Textbox& style,
		                    const sm::Matrix2D& mat, const Color& mul_col, const Color& add_col)
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