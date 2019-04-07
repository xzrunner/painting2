#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <SM_Vector.h>

#include <boost/signals2.hpp>

namespace pt2
{

class WindowContext
{
public:
	typedef boost::signals2::signal<void(const sm::vec2& offset, float scale)> OnView;
	typedef boost::signals2::signal<void(int width, int height)>               OnProj;

public:
	WindowContext();
	WindowContext(float proj_width, float proj_height, int screen_width, int screen_height);
    ~WindowContext();

	boost::signals2::connection DoOnView(const OnView::slot_type& slot);
	boost::signals2::connection DoOnProj(const OnProj::slot_type& slot);

	void SetView(const sm::vec2& offset, float scale);
	void SetProjection(int width, int height);
	void SetScreen(int width, int height);
	void SetViewport(int x, int y, int w, int h);

	void UpdateView() const;
	void UpdateProjection() const;
	void UpdateViewport() const;

	void Bind();

	const sm::vec2& GetViewOffset() const { return m_view_offset; }
	float GetViewScale() const { return m_view_scale; }

	int  GetScreenWidth() const { return m_screen_width; }
	int  GetScreenHeight() const { return m_screen_height; }

	float GetProjWidth() const { return m_proj_width; }
	float GetProjHeight() const { return m_proj_height; }

	void GetViewport(int& x, int& y, int& w, int& h) const {
		x = m_vp_x; y = m_vp_y; w = m_vp_w; h = m_vp_h;
	}

private:
	OnView m_on_view;
	OnProj m_on_proj;

	sm::vec2 m_view_offset;
	float    m_view_scale;

	float m_proj_width;
	float m_proj_height;

	int m_screen_width;
	int m_screen_height;

	int m_vp_x, m_vp_y, m_vp_w, m_vp_h;

}; // WindowContext

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif