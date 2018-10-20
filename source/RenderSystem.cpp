#include "painting2/RenderSystem.h"
#include "painting2/Texture.h"
#include "painting2/RenderColorCommon.h"
#include "painting2/RenderColorMap.h"
#include "painting2/Callback.h"
#include "painting2/DrawMask.h"
#include "painting2/SpriteRenderer.h"
#include "painting2/ColorRenderer.h"
#include "painting2/PrimitiveRenderer.h"

#include <unirender/Shader.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <tessellation/Painter.h>
#include <geoshape/Shape2D.h>

namespace pt2
{

CU_SINGLETON_DEFINITION(RenderSystem);

RenderSystem::RenderSystem()
{
	m_painter = std::make_unique<tess::Painter>();
}

void RenderSystem::SetPainterMat(const sm::mat4& pt_mat)
{
	if (m_pt_mat != pt_mat) {
		FlushPainter();
		m_pt_mat = pt_mat;
	}
}

void RenderSystem::FlushPainter() const
{
	static std::unique_ptr<PrimitiveRenderer> pr = nullptr;
	if (!pr) {
		pr = std::make_unique<PrimitiveRenderer>();
	}

	pr->Draw(*m_painter, m_pt_mat);
	m_painter->Clear();
}

void RenderSystem::DrawShape(const gs::Shape& shape, const sm::mat4& mat)
{
	SetPainterMat(mat);

	auto& s2 = static_cast<const gs::Shape2D&>(shape);
	s2.Draw([&](const sm::vec2* vertices, size_t n, bool close, bool filling) {
		if (close)
		{
			if (filling) {
				m_painter->AddPolygonFilled(vertices, n, 0xff0000ff);
			} else {
				m_painter->AddPolygon(vertices, n, 0xff0000ff);
			}
		}
		else
		{
			m_painter->AddPolyline(vertices, n, 0xff00ffff);
		}
	});
}

void RenderSystem::DrawTexture(const Texture& tex, const sm::rect& pos,
                               const sm::Matrix2D& mat)
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);

	float vertices[8];
	CalcVertices(pos, mat, vertices);

	float txmin, txmax, tymin, tymax;
	txmin = tymin = 0;
	txmax = tymax = 1;
	float texcoords[8] = {
		txmin, tymin,
		txmax, tymin,
		txmax, tymax,
		txmin, tymax,
	};

	auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->DrawQuad(vertices, texcoords, tex.TexID());
}

void RenderSystem::DrawTexture(const Texture& tex, const sm::mat4& mat)
{
	static std::unique_ptr<SpriteRenderer> sr = nullptr;
	if (!sr) {
		sr = std::make_unique<SpriteRenderer>();
	}

	sr->Draw(tex.TexID(), mat);
}

void RenderSystem::DrawTexture(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	static std::unique_ptr<SpriteRenderer> sr = nullptr;
	if (!sr) {
		sr = std::make_unique<SpriteRenderer>();
	}

	sr->Draw(shader, mat);
}

void RenderSystem::DrawColor(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	static std::unique_ptr<ColorRenderer> cr = nullptr;
	if (!cr) {
		cr = std::make_unique<ColorRenderer>();
	}

	cr->Draw(shader, mat);
}

void RenderSystem::DrawText(const std::string& text, const Textbox& style,
		                    const sm::Matrix2D& mat, const Color& mul_col, const Color& add_col)
{
	Callback::DrawText(text, style, mat, mul_col, add_col);
}

void RenderSystem::SetColor(const RenderColorCommon& col)
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	// todo: other shader
	auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());
}

void RenderSystem::SetColorMap(const RenderColorMap& col)
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	// todo: other shader
	auto shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColorMap(col.rmap.ToABGR(), col.gmap.ToABGR(), col.bmap.ToABGR());
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