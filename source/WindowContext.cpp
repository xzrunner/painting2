#include "painting2/WindowContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>

namespace pt2
{

WindowContext::WindowContext() 
	: m_mv_scale(0)
	, m_proj_width(0)
	, m_proj_height(0)
	, m_screen_width(0)
	, m_screen_height(0)
	, m_vp_x(0)
	, m_vp_y(0) 
	, m_vp_w(0)
	, m_vp_h(0)
{}

WindowContext::WindowContext(float proj_width, float proj_height, int screen_width, int screen_height)
	: m_mv_offset(0, 0)
	, m_mv_scale(1)
	, m_proj_width(proj_width)
	, m_proj_height(proj_height) 
	, m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_vp_x(0)
	, m_vp_y(0) 
	, m_vp_w(screen_width)
	, m_vp_h(screen_height)
{}

void WindowContext::SetModelView(const sm::vec2& offset, float scale)
{
	if (offset == m_mv_offset && scale == m_mv_scale) {
		return;
	} 

	m_mv_offset = offset;
	m_mv_scale  = scale;

	UpdateModelView();
}

void WindowContext::SetProjection(int width, int height)
{
	if (m_proj_width == width && m_proj_height == height) {
		return;
	}

	m_proj_width  = static_cast<float>(width);
	m_proj_height = static_cast<float>(height);

	UpdateProjection();
}

void WindowContext::SetScreen(int width, int height)
{
	m_screen_width  = width;
	m_screen_height = height;
}

void WindowContext::SetViewport(int x, int y, int w, int h)
{
	if (m_vp_x == x &&
		m_vp_y == y &&
		m_vp_w == w &&
		m_vp_h == h) {
		return;
	}

	m_vp_x = x;
	m_vp_y = y;
	m_vp_w = w;
	m_vp_h = h;
	UpdateViewport();

	m_proj_width  = static_cast<float>(w);
	m_proj_height = static_cast<float>(h);
	UpdateProjection();
}

void WindowContext::UpdateMVP() const
{
	UpdateModelView();
	UpdateProjection();
}

void WindowContext::UpdateModelView() const
{
	sl::Blackboard::Instance()->GetRenderContext().GetSubMVP2().
		NotifyModelview(m_mv_offset.x, m_mv_offset.y, m_mv_scale, m_mv_scale);
}

void WindowContext::UpdateProjection() const
{
	sl::Blackboard::Instance()->GetRenderContext().GetSubMVP2().
		NotifyProjection(static_cast<int>(m_proj_width), static_cast<int>(m_proj_height));
}

void WindowContext::UpdateViewport() const
{
	if (m_vp_w == 0 && m_vp_h == 0) {
		return;
	}

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.SetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

}