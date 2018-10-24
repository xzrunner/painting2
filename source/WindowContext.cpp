#include "painting2/WindowContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace pt2
{

WindowContext::WindowContext()
	: m_view_scale(0)
	, m_proj_width(0)
	, m_proj_height(0)
	, m_screen_width(0)
	, m_screen_height(0)
	, m_vp_x(0)
	, m_vp_y(0)
	, m_vp_w(0)
	, m_vp_h(0)
{
}

WindowContext::WindowContext(float proj_width, float proj_height, int screen_width, int screen_height)
	: m_view_offset(0, 0)
	, m_view_scale(1)
	, m_proj_width(proj_width)
	, m_proj_height(proj_height)
	, m_screen_width(screen_width)
	, m_screen_height(screen_height)
	, m_vp_x(0)
	, m_vp_y(0)
	, m_vp_w(screen_width)
	, m_vp_h(screen_height)
{
}

boost::signals2::connection WindowContext::DoOnView(const OnView::slot_type& slot)
{
	return m_on_view.connect(slot);
}

boost::signals2::connection WindowContext::DoOnProj(const OnProj::slot_type& slot)
{
	return m_on_proj.connect(slot);
}

void WindowContext::SetView(const sm::vec2& offset, float scale)
{
	if (offset == m_view_offset && scale == m_view_scale) {
		return;
	}

	m_view_offset = offset;
	m_view_scale  = scale;

	UpdateView();
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

void WindowContext::UpdateView() const
{
	m_on_view(m_view_offset, m_view_scale);
}

void WindowContext::UpdateProjection() const
{
	m_on_proj(m_proj_width, m_proj_height);
}

void WindowContext::UpdateViewport() const
{
	if (m_vp_w == 0 && m_vp_h == 0) {
		return;
	}

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.SetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

void WindowContext::Bind()
{
	UpdateView();
	UpdateProjection();
	UpdateViewport();
}

}