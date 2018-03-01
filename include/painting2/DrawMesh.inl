#pragma once

#include "painting2/config.h"
#include "painting2/PrimitiveDraw.h"
#include "painting2/RenderTargetMgr.h"
#include "painting2/RenderTarget.h"
#include "painting2/RenderScissor.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"
#include "painting2/WindowContext.h"
#include "painting2/Color.h"

#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/Sprite2Shader.h>
#ifdef PT2_DISABLE_DEFERRED
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#else
#include <cooking/Facade.h>
#include <cooking/DisplayList.h>
#endif // PT2_DISABLE_DEFERRED
#ifndef PT2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#endif // PT2_DISABLE_STATISTICS

namespace
{

void draw_sprite2(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id)
{
#ifdef PT2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	assert(shader_mgr.GetShaderType() == sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
	shader->DrawQuad(positions, texcoords, tex_id);
#else
	cooking::draw_quad_sprite(dlist, positions, texcoords, tex_id);
#endif // PT2_DISABLE_DEFERRED
}

void draw_filter(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id)
{
#ifdef PT2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	assert(shader_mgr.GetShaderType() == sl::FILTER);

	sl::FilterShader* shader = static_cast<sl::FilterShader*>(shader_mgr.GetShader());
	shader->Draw(positions, texcoords, tex_id);
#else
	cooking::draw_quad_filter(dlist, positions, texcoords, tex_id);
#endif // PT2_DISABLE_DEFERRED
}

}

namespace pt2
{

static Color RED	(204, 51, 102, 128);
static Color GREEN	(102, 204, 51, 128);
static Color BLUE	(102, 51, 204, 128);

template<typename T, typename Params>
DrawMesh<T, Params>::DrawMesh(const Mesh<T>& m_mesh)
	: m_mesh(m_mesh)
{
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
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

	// lines
	PrimitiveDraw::SetColor(RED);
	CU_VEC<sm::vec2> lines;
	lines.resize(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j].x = (texcoords[triangles[i]].x - 0.5f) * w;
			lines[j].y = (texcoords[triangles[i]].y - 0.5f) * h;
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		PrimitiveDraw::Polyline(dlist, lines, true);
	}

	// points
	PrimitiveDraw::SetColor(BLUE);
	for (int i = 0, n = texcoords.size(); i < n; ++i) {
		sm::vec2 p;
		p.x = (texcoords[i].x - 0.5f) * w;
		p.y = (texcoords[i].y - 0.5f) * h;
		if (mt) {
			p = *mt * p;
		}
		PrimitiveDraw::Circle(dlist, p, m_mesh.GetNodeRadius(), true);
	}

	return RENDER_OK;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawInfoXY(cooking::DisplayList* dlist, const sm::Matrix2D* mt)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

	// lines
	PrimitiveDraw::SetColor(RED);
	CU_VEC<sm::vec2> lines;
	lines.resize(3);
	for (int i = 0, n = triangles.size(); i < n; ) {
		for (int j = 0; j < 3; ++j, ++i) {
			lines[j] = vertices[triangles[i]];
			if (mt) {
				lines[j] = *mt * lines[j];
			}
		}
		PrimitiveDraw::Polyline(dlist, lines, true);
	}

	// points
	PrimitiveDraw::SetColor(BLUE);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		sm::vec2 p = vertices[i];
		if (mt) {
			p = *mt * p;
		}
		PrimitiveDraw::Circle(dlist, p, m_mesh.GetNodeRadius(), true);
	}

	return RENDER_OK;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawTexture(cooking::DisplayList* dlist, const Params& params, const std::shared_ptr<const T>& base_sym)
{
	RenderReturn ret = RENDER_OK;
	auto& node = base_sym ? base_sym : m_mesh.GetBaseSymbol();
	if (IsNodeImage(*node))
	{
		float texcoords[8];
		int tex_id;
		auto ret = PrepareDrawOnePass(dlist, *node, params, texcoords, &tex_id);
		if (ret != RENDER_OK) {
			return ret;
		}
		ret = DrawOnePass(dlist, params, texcoords, tex_id);
	} 
	else 
	{
		ret = DrawTwoPass(dlist, params, *node);

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

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawOnlyMesh(cooking::DisplayList* dlist, const sm::Matrix2D& mt, int tex_id)
{
	CU_VEC<sm::vec2> vertices, texcoords;
	CU_VEC<int> triangles;
	m_mesh.DumpToTriangles(vertices, texcoords, triangles);
	if (triangles.empty()) {
		return RENDER_NO_DATA;
	}

#ifdef PT2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();

	shader_mgr.SetShader(sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
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
		shader->DrawQuad(&_vertices[0].x, &_texcoords[0].x, tex_id);
#else
		cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // PT2_DISABLE_DEFERRED
	}

	return RENDER_OK;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawOnePass(cooking::DisplayList* dlist, const Params& params, const float* src_texcoords, int tex_id)
{
	sl::ShaderType shader_type;
#ifdef PT2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_type = shader_mgr.GetShaderType();
	if (shader_type != sl::SPRITE2 && shader_type != sl::FILTER) {
		return RENDER_NO_DATA;
	}
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

	void (*draw)(cooking::DisplayList* dlist, const float* positions, const float* texcoords, int tex_id) = nullptr;

	switch (shader_type)
	{
		case sl::SPRITE2:
		{
#ifdef PT2_DISABLE_DEFERRED
			sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader());
			shader->SetColor(params.col_common.mul.ToABGR(), params.col_common.add.ToABGR());
			shader->SetColorMap(params.col_map.rmap.ToABGR(), params.col_map.gmap.ToABGR(), params.col_map.bmap.ToABGR());
#else
			cooking::set_color_sprite(dlist, params.col_common.mul.ToABGR(), params.col_common.add.ToABGR(),
				params.col_map.rmap.ToABGR(), params.col_map.gmap.ToABGR(), params.col_map.bmap.ToABGR());
#endif // PT2_DISABLE_DEFERRED
			draw = draw_sprite2;
		}
			break;
		case sl::FILTER:
		{
			auto& col = params.col_common;
#ifdef PT2_DISABLE_DEFERRED
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(shader_mgr.GetShader());
			shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());
#else
			cooking::set_color_filter(dlist, col.mul.ToABGR(), col.add.ToABGR());
#endif // PT2_DISABLE_DEFERRED
			draw = draw_filter;
		}
			break;
	}

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
				_vertices[j] = params.mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].x;
				_texcoords[j].y = y + h * texcoords[idx].y;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
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
				_vertices[j] = params.mt * vertices[idx];
				_texcoords[j].x = x + w * texcoords[idx].y;
				_texcoords[j].y = y + h * texcoords[idx].x;
			}
			_vertices[3] = _vertices[2];
			_texcoords[3] = _texcoords[2];

			draw(dlist, &_vertices[0].x, &_texcoords[0].x, tex_id);
		}
	}
	else
	{
		assert(0);
	}

	return RENDER_OK;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawTwoPass(cooking::DisplayList* dlist, const Params& params, const T& node)
{
	auto& rc = Blackboard::Instance()->GetRenderContext();
	auto& rt_mgr = rc.GetRTMgr();
	RenderTarget* rt = rt_mgr.Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MESH);
#endif // S2_DISABLE_STATISTICS

	sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr().FlushShader();

	rc.GetScissor().Disable();

	auto old_wc = pt2::Blackboard::Instance()->GetWindowContext();
	auto new_wc = std::make_shared<pt2::WindowContext>(
		static_cast<float>(rt_mgr.WIDTH), static_cast<float>(rt_mgr.HEIGHT), rt_mgr.WIDTH, rt_mgr.HEIGHT);
	new_wc->Bind();
	pt2::Blackboard::Instance()->SetWindowContext(new_wc);

	ret |= DrawMesh2RT(dlist, rt, params, node);

	old_wc->Bind();
	pt2::Blackboard::Instance()->SetWindowContext(old_wc);

	rc.GetScissor().Enable();

	ret |= DrawRT2Screen(dlist, rt, params.mt);

	rt_mgr.Return(rt);

	return ret;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawMesh2RT(cooking::DisplayList* dlist, RenderTarget* rt, const Params& params, const T& node)
{
	rt->Bind();

#ifdef PT2_DISABLE_DEFERRED
	ur::Blackboard::Instance()->GetRenderContext().Clear(0);
#else
	cooking::render_clear(dlist, 0);
#endif // PT2_DISABLE_DEFERRED

	RenderReturn ret = DrawNode(node, params);

#ifdef PT2_DISABLE_DEFERRED
	sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr().FlushShader();
#else
	cooking::flush_shader(dlist);
#endif // PT2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

template<typename T, typename Params>
RenderReturn DrawMesh<T, Params>::
DrawRT2Screen(cooking::DisplayList* dlist, RenderTarget* rt, const sm::Matrix2D& mt)
{
	return DrawOnlyMesh(dlist, mt, rt->GetTexID());
}

}