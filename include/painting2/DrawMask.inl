#pragma once

#include "painting2/config.h"
#include "painting2/RenderTargetMgr.h"
#include "painting2/RenderTarget.h"
#include "painting2/RenderScissor.h"
#include "painting2/RenderCtxStack.h"

#include <stat/StatPingPong.h>
#include <stat/StatOverdraw.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/MaskShader.h>
#include <unirender/RenderContext.h>
#ifndef PT2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED

namespace pt2
{

template<typename Type, typename Params>
DrawMask<Type, Params>::DrawMask(const Type& base, const Type& mask, const Params& params)
	: m_base(base)
	, m_mask(mask)
	, m_params(params)
{
}

template<typename Type, typename Params>
RenderReturn DrawMask<Type, Params>::DrawImpl(cooking::DisplayList* dlist) const
{
	pt2::RenderReturn ret = pt2::RENDER_OK;

#ifndef PT2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MASK);
#endif // PT2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();

#ifdef PT2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::Blackboard::Instance()->GetShaderMgr();
	mgr->FlushShader();
#else
	cooking::flush_shader(dlist);
#endif // PT2_DISABLE_DEFERRED

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	RenderTarget* rt_base = RT->Fetch();
	if (!rt_base) {
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawBaseToRT(dlist, rt_base);

	RenderTarget* rt_mask = RT->Fetch();
	if (!rt_mask) {
		RT->Return(rt_base);
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return RENDER_NO_RT;
	}
	ret |= DrawMaskToRT(dlist, rt_mask);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	ret |= DrawMaskFromRT(dlist, rt_base, rt_mask);

	RT->Return(rt_base);
	RT->Return(rt_mask);

	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMask<Type, Params>::DrawBaseToRT(cooking::DisplayList* dlist, RenderTarget* rt) const
{
	rt->Bind();

#ifdef PT2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::Blackboard::Instance()->GetShaderMgr();

	mgr->GetContext().Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();
#else
	cooking::render_clear(dlist, 0);
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // PT2_DISABLE_DEFERRED

	pt2::RenderReturn ret = DrawBaseNode(m_base, m_params);

#ifdef PT2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // PT2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMask<Type, Params>::DrawMaskToRT(cooking::DisplayList* dlist, RenderTarget* rt) const
{
	rt->Bind();

#ifdef PT2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::Blackboard::Instance()->GetShaderMgr();

	mgr->GetContext().Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();
#else
	cooking::render_clear(dlist, 0);
	cooking::change_shader(dlist, sl::SPRITE2);
#endif // PT2_DISABLE_DEFERRED

	pt2::RenderReturn ret = DrawMaskNode(m_mask, m_params);

#ifdef PT2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // PT2_DISABLE_DEFERRED

	rt->Unbind();

	return ret;
}

template<typename Type, typename Params>
RenderReturn DrawMask<Type, Params>::
DrawMaskFromRT(cooking::DisplayList* dlist, RenderTarget* rt_base, RenderTarget* rt_mask) const
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sm::vec2 vertices[4];
	sm::rect r = GetBounding(m_mask);;
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);

	sm::vec2 texcoords[4];
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i];
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i];
		texcoords_mask[i].x = texcoords_mask[i].x / RT->WIDTH  + 0.5f;
		texcoords_mask[i].y = texcoords_mask[i].y / RT->HEIGHT + 0.5f;
	}

	auto& mt = GetMat(m_params);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

#ifndef PT2_DISABLE_STATISTICS
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax =-FLT_MAX, ymax =-FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		const sm::vec2& pos = vertices[i];
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	const RenderContext* ctx = RenderCtxStack::Instance()->Top();
	if (ctx) {
		float area = (xmax - xmin) * (ymax - ymin) / ctx->GetScreenWidth() / ctx->GetScreenHeight();
		st::StatOverdraw::Instance()->AddArea(area);
	}
#endif // PT2_DISABLE_STATISTICS

#ifdef PT2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::Blackboard::Instance()->GetShaderMgr();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#else
	cooking::change_shader(dlist, sl::MASK);
	cooking::draw_quad_mask(dlist, &vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
#endif // PT2_DISABLE_DEFERRED

	return RENDER_OK;
}

//RenderReturn DrawMask::DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp)
//{
//	const Actor* base_actor = base->QueryActor(rp.actor);
//	bool visible = base_actor ? base_actor->IsVisible() : base->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//	const Actor* mask_actor = mask->QueryActor(rp.actor);
//	visible = mask_actor ? mask_actor->IsVisible() : mask->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//
//	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
//
//	rc->ClearAll();
//
//	rc->ColorMask(false, false, false, true);
//
//	rc->EnableStencil(true);
//	rc->EnableAlpha(true);
//
//	rc->StencilFunc(ur::RenderContext::ST_ALWAYS, 0, 0);
//	rc->StencilOp(ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP, ur::RenderContext::ST_INCR);	
//	rc->AlphaFunc();
//
//	RenderParams rp_child = rp;
//	rp_child.actor = mask_actor;
//	DrawNode::Draw(mask, rp_child);
//	sl::ShaderMgr::Instance()->FlushShader();
//
//	rc->EnableAlpha(false);
//	
//	rc->ColorMask(true, true, true, true);
//	
//// 	rc->StencilFunc(ur::RenderContext::ST_LESS, 1, -1);
//	rc->StencilFunc(ur::RenderContext::ST_NEVER, 0, 0);
//
// 	rp_child.actor = base_actor;
// 	DrawNode::Draw(base, rp_child);
// 	sl::ShaderMgr::Instance()->FlushShader();
//
//	rc->EnableStencil(false);
//
//	return RENDER_OK;
//}

//RenderReturn DrawMask::DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp)
//{
//	const Actor* base_actor = base->QueryActor(rp.actor);
//	bool visible = base_actor ? base_actor->IsVisible() : base->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//	const Actor* mask_actor = mask->QueryActor(rp.actor);
//	visible = mask_actor ? mask_actor->IsVisible() : mask->IsVisible();
//	if (!visible) {
//		return RENDER_INVISIBLE;
//	}
//
//	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
//
//	rc->EnableStencil(true);
//	rc->ClearStencil(0);
//
//	rc->ClearAll();
//	{
//		rc->ColorMask(false, false, false, true);
//
//		rc->StencilFunc(ur::RenderContext::ST_NEVER, 0, 0);
//		rc->StencilOp(ur::RenderContext::ST_INCR, ur::RenderContext::ST_INCR, ur::RenderContext::ST_INCR);
//
//		rc->EnableAlpha(true);
//		rc->AlphaFunc();
//
//		RenderParams rp_child = rp;
//		rp_child.actor = mask_actor;
//		DrawNode::Draw(mask, rp_child);
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		rc->EnableAlpha(false);
//		rc->ColorMask(true, true, true, true);
//	}
//
//	{
//		rc->StencilFunc(ur::RenderContext::ST_NOTEQUAL, 0, 1);
//		rc->StencilOp(ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP, ur::RenderContext::ST_KEEP);
//
//		RenderParams rp_child = rp;
//		rp_child.actor = base_actor;
//		DrawNode::Draw(base, rp_child);
//		sl::ShaderMgr::Instance()->FlushShader();
//
//		sl::ShaderMgr::Instance()->FlushShader();
//	}
//
//	rc->EnableStencil(false);
//
//	return RENDER_OK;
//}

}