#include "painting2/OrthoCamera.h"
#include "painting2/WindowContext.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"

namespace pt2
{

OrthoCamera::OrthoCamera()
	: m_position(0, 0)
	, m_scale(1)
{
	UpdateRender();
}

void OrthoCamera::OnSize(float width, float height)
{
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	if (wc) {
		wc->SetProjection(static_cast<int>(width), static_cast<int>(height));
		wc->SetScreen(static_cast<int>(width), static_cast<int>(height));
		// todo: set viewport single
//		wc->SetViewport(0, 0, width, height);
	}
}

void OrthoCamera::Bind() const
{
	UpdateRender();
}

void OrthoCamera::Reset()
{
	m_position.Set(0, 0);
	m_scale = 1;

	UpdateRender();
}

sm::vec2 OrthoCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm::vec2 proj;
	const int vx = x, vy = height - y;
	proj.x = (vx - (width >> 1))  * m_scale + m_position.x;
	proj.y = (vy - (height >> 1)) * m_scale + m_position.y;
	return proj;
}

sm::vec2 OrthoCamera::TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const
{
	sm::vec2 scr;
	float vx = (proj.x - m_position.x) / m_scale + (width >> 1),
		  vy = (proj.y - m_position.y) / m_scale + (height >> 1);
	scr.x = vx;
	scr.y = height - vy;
	return scr;
}

void OrthoCamera::Translate(const sm::vec2& offset)
{
	m_position += offset * m_scale;

	UpdateRender();
}

void OrthoCamera::Scale(float scale, int x, int y, int width, int height)
{
	float new_scale = m_scale * scale;
	m_position.x = (x - (width >> 1)) * m_scale + m_position.x - (x - (width >> 1)) * new_scale;
	m_position.y = (y - (height >> 1)) * m_scale + m_position.y - (y - (height >> 1)) * new_scale;
	m_scale = new_scale;

	UpdateRender();
}

void OrthoCamera::Set(const sm::vec2& pos, float scale)
{
	if (m_position == pos && m_scale == scale) {
		return;
	}

	m_position = pos;
	m_scale = scale;

	UpdateRender();
}

void OrthoCamera::UpdateRender() const
{
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	if (!wc) {
		return;
	}

	sm::vec2 mv_offset = - m_position;
	float mv_scale = 1 / m_scale;
	wc->SetModelView(mv_offset, mv_scale);
}

}