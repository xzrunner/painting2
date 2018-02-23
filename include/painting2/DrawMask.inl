#pragma once

#include "painting2/RenderTargetMgr.h"
#include "painting2/RenderScissor.h"
#include "painting2/RenderCtxStack.h"

#include <stat/StatPingPong.h>
#include <shaderlab/ShaderMgr.h>

namespace pt2
{

template<typename Type, typename Params>
void DrawMask::Draw(const Type& base, const Type& mask, const Params& params,
	                std::function<void(const Type& data, const Params& params)> draw_cb)
{
#ifndef S2_DISABLE_STATISTICS
	st::StatPingPong::Instance()->AddCount(st::StatPingPong::MASK);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	RenderTarget* rt_base = RT->Fetch();
	if (!rt_base) {
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return;
	}
	DrawBaseToRT(rt_base, base, params, draw_cb);

	RenderTarget* rt_mask = RT->Fetch();
	if (!rt_mask) {
		RT->Return(rt_base);
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Enable();
		return;
	}
	DrawMaskToRT(rt_mask, mask, params, draw_cb);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	DrawMaskFromRT(rt_base, rt_mask, mask, params);

	RT->Return(rt_base);
	RT->Return(rt_mask);
}

template<typename Type, typename Params>
void DrawMask::DrawBaseToRT(RenderTarget* rt, const Type& base, const Params& params,
	                        std::function<void(const Type& data, const Params& params)> draw_cb)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	//RenderParamsProxy rp_proxy;
	//RenderParams* rp_child = rp_proxy.obj;
	//rp_child->Reset();

	//rp_child->col_common = rp.col_common;
	//rp_child->col_map = rp.col_map;
	//rp_child->actor = actor;
	//rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
	////	rp_child->mt = rp.mt;

	draw_cb(base, params);

	shader->Commit();

	rt->Unbind();
}

template<typename Type, typename Params>
void DrawMask::DrawMaskToRT(RenderTarget* rt, const Type& mask, const Params& params,
	                        std::function<void(const Type& data, const Params& params)> draw_cb)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	//RenderParamsProxy rp_proxy;
	//RenderParams* rp_child = rp_proxy.obj;
	//rp_child->Reset();

	//rp_child->SetChangeShader(false);
	//rp_child->actor = actor;
	//rp_child->SetDisableDTexC2(rp.IsDisableDTexC2());
	////	rp_child->mt = rp.mt;

	draw_cb(mask, params);

	shader->Commit();

	rt->Unbind();
}

template<typename Type, typename Params>
void DrawMask::DrawMaskFromRT(RenderTarget* rt_base, RenderTarget* rt_mask, 
	                          const Type& mask, const Params& params)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sm::vec2 vertices[4];
	sm::rect r = mask->GetSymbol()->GetBounding();
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

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

#ifndef S2_DISABLE_STATISTICS
	float xmin = FLT_MAX, ymin = FLT_MAX,
		  xmax =-FLT_MAX, ymax =-FLT_MAX;
	for (int i = 0; i < 4; ++i) {
		const sm::vec2& pos = vertices[i];
		if (pos.x < xmin) xmin = pos.x;
		if (pos.x > xmax) xmax = pos.x;
		if (pos.y < ymin) ymin = pos.y;
		if (pos.y > ymax) ymax = pos.y;
	}
	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();	
	float area = (xmax - xmin) * (ymax - ymin) / sz.x / sz.y;
	StatOverdraw::Instance()->AddArea(area);
#endif // S2_DISABLE_STATISTICS

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, rt_base->GetTexID(), rt_mask->GetTexID());
}

}