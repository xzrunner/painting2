#include "painting2/RenderScissor.h"
#include "painting2/RenderScreen.h"

#include <unirender/RenderContext.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <SM_Test.h>

#include <assert.h>

namespace pt2
{

void RenderScissor::Push(float x, float y, float w, float h, bool use_render_screen, bool no_intersect)
{
	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetShaderMgr().FlushShader();

	ur::RenderContext& ur_rc = rc.GetContext();
	ur_rc.EnableScissor(true);

	if (!no_intersect && !m_stack.empty() && !m_stack.back().IsInvalid()) {
		Intersection(m_stack.back(), x, y, w, h);
	}

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	r.use_render_screen = use_render_screen;
	m_stack.push_back(r);

	if (use_render_screen) {
		RenderScreen::Scissor(x, y, w, h);
	} else {
		ur_rc.SetScissor(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
	}
}

void RenderScissor::Pop()
{
	assert(!m_stack.empty());

	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetShaderMgr().FlushShader();
	m_stack.pop_back();
	ur::RenderContext& ur_rc = rc.GetContext();
	if (m_stack.empty()) {
		ur_rc.EnableScissor(false);
		return;
	} else {
		ur_rc.EnableScissor(true);
	}

	const Rect& r = m_stack.back();
	if (r.IsInvalid()) {
		ur_rc.EnableScissor(false);
		return;
	}
	if (r.use_render_screen) {
		RenderScreen::Scissor(r.x, r.y, r.w, r.h);
	} else {
		ur_rc.SetScissor(static_cast<int>(r.x), static_cast<int>(r.y), static_cast<int>(r.w), static_cast<int>(r.h));
	}
}

bool RenderScissor::IsEmpty() const 
{ 
	return m_stack.empty() || m_stack.back().IsInvalid();
}

void RenderScissor::Disable()
{
	auto& ur_rc = sl::Blackboard::Instance()->GetRenderContext().GetContext();
	ur_rc.EnableScissor(false);
	Rect r;
	r.MakeInvalid();
	m_stack.push_back(r);
}

void RenderScissor::Enable()
{
	Pop();
}

bool RenderScissor::CullingTestOutside(const sm::rect& r) const
{
	if (m_stack.empty()) {
		return false;
	}
	const Rect& rr = m_stack.back();
	if (rr.IsInvalid()) {
		return true;
	} else {
		return !sm::is_rect_intersect_rect(sm::rect(rr.x, rr.y, rr.x + rr.w, rr.y + rr.h), r);
	}
}

void RenderScissor::Intersection(const Rect& r, float& x, float& y, float& w, float& h)
{
	float newx = r.x > x ? r.x : x,
		  newy = r.y > y ? r.y : y;

	float bx = r.x + r.w,
		  by = r.y + r.h;
	float ax = x + w,
		  ay = y + h;
	float neww = (bx > ax ? ax : bx) - newx,
		  newh = (by > ay ? ay : by) - newy;

	x = newx;
	y = newy;
	w = neww;
	h = newh;
}

}