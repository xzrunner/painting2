#pragma once

#include "painting2/config.h"
#include "painting2/RenderTargetMgr.h"
#include "painting2/RenderTarget.h"
#include "painting2/RenderScissor.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"
#include "painting2/WindowContext.h"
#include "painting2/RenderSystem.h"

#ifdef PT2_DISABLE_DEFERRED
#else
#include <cooking/Facade.h>
#include <cooking/DisplayList.h>
#endif // PT2_DISABLE_DEFERRED
#ifndef PT2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#endif // PT2_DISABLE_STATISTICS
#include <painting0/Color.h>
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/SpriteRenderer.h>
#include <tessellation/Painter.h>

namespace
{

void draw_sprite2(const ur2::Device& dev, ur2::Context& ctx, cooking::DisplayList* dlist,
                  const float* positions, const float* texcoords, int tex_id)
{
#ifdef PT2_DISABLE_DEFERRED
	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
    ur2::RenderState rs;
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, positions, texcoords, tex_id, 0xffffffff);
#else
	cooking::draw_quad_sprite(dlist, positions, texcoords, tex_id);
#endif // PT2_DISABLE_DEFERRED
}

void draw_filter(const ur2::Device& dev, ur2::Context& ctx, ur2::RenderState& rs,
                 cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id)
{
#ifdef PT2_DISABLE_DEFERRED
	// fixme: filter
	auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
	std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, positions, texcoords, tex_id, 0xffffffff);
#else
	cooking::draw_quad_filter(dlist, positions, texcoords, tex_id);
#endif // PT2_DISABLE_DEFERRED
}

}

namespace pt2
{

static pt0::Color RED	(204, 51, 102, 128);
static pt0::Color GREEN	(102, 204, 51, 128);
static pt0::Color BLUE	(102, 51, 204, 128);

template<typename Type, typename Params>
DrawMesh<Type, Params>::DrawMesh(const Mesh<Type>& m_mesh)
	: m_mesh(m_mesh)
{
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawInfoUV(cooking::DisplayList* dlist, const sm::Matrix2D* mt)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	float w = m_mesh.GetWidth(),
		  h = m_mesh.GetHeight();

	tess::Painter pt;

	// lines
	uint32_t col = RED.ToABGR();
	std::array<sm::vec2, 3> lines;
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j].x = (texcoords[triangles[i]].x - 0.5f) * w;
			lines[j].y = (texcoords[triangles[i]].y - 0.5f) * h;
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		pt.AddPolygon(lines.data(), lines.size(), col);
	}

	// points
	col = BLUE.ToABGR();
	for (int i = 0, n = texcoords.size(); i < n; ++i) {
		sm::vec2 p;
		p.x = (texcoords[i].x - 0.5f) * w;
		p.y = (texcoords[i].y - 0.5f) * h;
		if (mt) {
			p = *mt * p;
		}
		pt.AddCircleFilled(p, m_mesh.GetNodeRadius(), col);
	}

	RenderSystem::DrawPainter(pt);

	return RENDER_OK;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawInfoXY(cooking::DisplayList* dlist, const sm::Matrix2D* mt)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	tess::Painter pt;

	// lines
	uint32_t col = RED.ToABGR();
	std::array<sm::vec2, 3> lines;
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j] = vertices[triangles[i]];
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		pt.AddPolygon(lines.data(), lines.size(), col);
	}

	// points
	col = BLUE.ToABGR();
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sm::vec2 p = vertices[i];
		if (mt) {
			p = *mt * p;
		}
		pt.AddCircleFilled(p, m_mesh.GetNodeRadius(), col);
	}

	pt2::RenderSystem::DrawPainter(pt);

	return RENDER_OK;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawTexture(const ur2::Device& dev, ur2::Context& ctx, cooking::DisplayList* dlist,
            const Params& params, const Type& base_sym)
{
	RenderReturn ret = RENDER_OK;
	auto& node = base_sym ? base_sym : m_mesh.GetBaseSymbol();
	if (IsNodeImage(node))
	{
		float texcoords[8];
		int tex_id;
		auto ret = PrepareDrawOnePass(dlist, node, params, texcoords, &tex_id);
		if (ret != RENDER_OK) {
			return ret;
		}
		ret = DrawOnePass(dev, ctx, dlist, params, texcoords, tex_id);
	}
	else
	{
		ret = DrawTwoPass(dlist, params, node);

		//////////////////////////////////////////////////////////////////////////

//  		int tex_id;
//  		const float* texcoords = DrawNode::DTexQuerySym(node, tex_id);
//  		if (texcoords) {
//  			DrawOnePass(m_mesh, params, texcoords, tex_id);
//  		} else {
//  			DrawNode::DTexCacheSym(node);
// 			DrawTwoPass(m_mesh, params, node);
// 		}
	}
	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawOnlyMesh(cooking::DisplayList* dlist, const sm::Matrix2D& mt, int tex_id)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

#ifdef PT2_DISABLE_DEFERRED
	auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SPRITE);
#else
	cooking::change_shader(dlist, sl::SPRITE2);
	cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
#endif // PT2_DISABLE_DEFERRED

	auto& rt_mgr = Blackboard::Instance()->GetRenderContext().GetRTMgr();
	int w = rt_mgr.WIDTH,
		h = rt_mgr.HEIGHT;
	float ori_w = m_mesh.GetWidth(),
		  ori_h = m_mesh.GetHeight();
	for (int i = 0, n = triangles.size(); i < n; )
	{
		sm::vec2 _vertices[4], _texcoords[4];
		for (int j = 0; j < 3; ++j, ++i)
		{
			int idx = triangles[i];
			_vertices[j] = mt * vertices[idx];
			_texcoords[j].x = (texcoords[idx].x * ori_w - ori_w * 0.5f + w * 0.5f) / w;
			_texcoords[j].y = (texcoords[idx].y * ori_h - ori_h * 0.5f + h * 0.5f) / h;
		}
		_vertices[3] = _vertices[2];
		_texcoords[3] = _texcoords[2];

#ifdef PT2_DISABLE_DEFERRED
		std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(&_vertices[0].x, &_texcoords[0].x, tex_id, 0xffffffff);
#else
		cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // PT2_DISABLE_DEFERRED
	}

	return RENDER_OK;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawOnePass(const ur2::Device& dev, ur2::Context& ctx, cooking::DisplayList* dlist, const Params& params, const float* src_texcoords, int tex_id)
{
//	sl::ShaderType shader_type;
#ifdef PT2_DISABLE_DEFERRED
	//auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	//shader_type = shader_mgr.GetShaderType();
	//if (shader_type != sl::SPRITE2 && shader_type != sl::FILTER) {
	//	return RENDER_NO_DATA;
	//}
#else
	int _shader_type = dlist->GetShaderType();
	assert(_shader_type >= 0);
	shader_type = static_cast<sl::ShaderType>(_shader_type);
	if (shader_type != sl::SPRITE2 && shader_type != sl::FILTER) {
		return RENDER_NO_DATA;
	}
#endif // PT2_DISABLE_DEFERRED

	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	float x = src_texcoords[0], y = src_texcoords[1];
	float w = src_texcoords[4] - src_texcoords[0],
		  h = src_texcoords[5] - src_texcoords[1];

	void (*draw)(const ur2::Device& dev, ur2::Context& ctx, cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id) = nullptr;

	//switch (shader_type)
	//{
	//	case sl::SPRITE2:
	//	{
#ifdef PT2_DISABLE_DEFERRED
			auto rd = rp::RenderMgr::Instance()->SetRenderer(dev, ctx, rp::RenderType::SPRITE);
            ur2::RenderState rs;
			std::static_pointer_cast<rp::SpriteRenderer>(rd)->DrawQuad(ctx, rs, &vertices[0].x, &texcoords[0].x, tex_id, 0xffffffff);
#else
			cooking::set_color_sprite(dlist, params.col_common.mul.ToABGR(), params.col_common.add.ToABGR(),
				params.col_map.rmap.ToABGR(), params.col_map.gmap.ToABGR(), params.col_map.bmap.ToABGR());
#endif // PT2_DISABLE_DEFERRED
			draw = draw_sprite2;
		//}
		//	break;
		//case sl::FILTER:
		//{
//			auto& col = params.col_common;
//#ifdef PT2_DISABLE_DEFERRED
//			sl::FilterShader* shader = static_cast<sl::FilterShader*>(shader_mgr.GetShader());
//			shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());
//#else
//			cooking::set_color_filter(dlist, col.mul.ToABGR(), col.add.ToABGR());
//#endif // PT2_DISABLE_DEFERRED
//			draw = draw_filter;
	//	}
	//		break;
	//}

	auto& mt = GetMat(params);

	// 3 2    1 0
	// 0 1 or 2 3
	if ((w > 0 && h > 0) || (w < 0 && h < 0))
	{
		for (int i = 0, n = triangles.size(); i < n; )
		{
			sm::vec2 _vertices[4], _texcoords[4];
			for (int j = 0; j < 3; ++j, ++i)
			{
				int idx = triangles[i];
				_vertices[j] = mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].x;
				_texcoords[j].y = y + h * texcoords[idx].y;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(dev, ctx, dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	// 0 3
	// 1 2
	else if (w > 0 && h < 0)
	{
		for (int i = 0, n = triangles.size(); i < n; )
		{
			sm::vec2 _vertices[4], _texcoords[4];
			for (int j = 0; j < 3; ++j, ++i)
			{
				int idx = triangles[i];
				_vertices[j] = mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].y;
				_texcoords[j].y = y + h * texcoords[idx].x;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(dev, ctx, dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	else
	{
		assert(0);
	}

	return RENDER_OK;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawTwoPass(cooking::DisplayList* dlist, const Params& params, const Type& node)
{
    // todo
    return RENDER_NO_RT;

//	auto& rc = Blackboard::Instance()->GetRenderContext();
//	auto& rt_mgr = rc.GetRTMgr();
//	auto rt = rt_mgr.Fetch();
//	if (!rt) {
//		return RENDER_NO_RT;
//	}
//
//	RenderReturn ret = RENDER_OK;
//
//#ifndef S2_DISABLE_STATISTICS
//	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MESH);
//#endif // S2_DISABLE_STATISTICS
//
//	rp::RenderMgr::Instance()->Flush();
//
//	rc.GetScissor().Disable();
//	{
//		pt2::WindowCtxRegion wcr(static_cast<float>(rt_mgr.WIDTH), static_cast<float>(rt_mgr.HEIGHT));
//		ret |= DrawMesh2RT(dlist, *rt, params, node);
//	}
//	rc.GetScissor().Enable();
//
//	ret |= DrawRT2Screen(dlist, *rt, GetMat(params));
//
//	rt_mgr.Return(rt);
//
//	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawMesh2RT(cooking::DisplayList* dlist, const std::shared_ptr<ur2::Framebuffer>& rt, const Params& params, const Type& node)
{
	rt.Bind();

#ifdef PT2_DISABLE_DEFERRED
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.SetClearColor(0);
	ur_rc.Clear();
#else
	cooking::render_clear(dlist, 0);
#endif // PT2_DISABLE_DEFERRED

	RenderReturn ret = DrawNode(node, params);

#ifdef PT2_DISABLE_DEFERRED
	rp::RenderMgr::Instance()->Flush();
#else
	cooking::flush_shader(dlist);
#endif // PT2_DISABLE_DEFERRED

	rt.Unbind();

	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMesh<Type, Params>::
DrawRT2Screen(cooking::DisplayList* dlist, const std::shared_ptr<ur2::Framebuffer>& rt, const sm::Matrix2D& mt)
{
	return DrawOnlyMesh(dlist, mt, rt.GetTexID());
}

}