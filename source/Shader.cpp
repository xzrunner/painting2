#include "painting2/Shader.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>

namespace pt2
{

Shader::Shader(ur::RenderContext* rc, const pt0::Shader::Params& params)
	: pt0::Shader(rc, params)
{
}

Shader::~Shader()
{
    ClearNotifies();
}

void Shader::UpdateViewMat(const sm::vec2& offset, float scale)
{
	if (offset == m_offset && scale == m_scale) {
		return;
	}
	m_offset = offset;
	m_scale = scale;

	Use();

    auto s_mat = sm::mat4::Scaled(scale, scale, 1);
    auto t_mat = sm::mat4::Translated(offset.x, offset.y, 1);
    auto mat = s_mat * t_mat;

    assert(!m_uniform_names[pt0::U_VIEW_MAT].empty());
	SetMat4(m_uniform_names[pt0::U_VIEW_MAT].c_str(), mat.x);
}

void Shader::UpdateProjMat(int width, int height)
{
	if (m_width == width && m_height == height) {
		return;
	}
	m_width = width;
	m_height = height;

	Use();

	float hw = width * 0.5f;
	float hh = height * 0.5f;
	auto mat = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);

    assert(!m_uniform_names[pt0::U_PROJ_MAT].empty());
	SetMat4(m_uniform_names[pt0::U_PROJ_MAT], mat.x);
}

void Shader::AddNotify(std::shared_ptr<WindowContext>& wc)
{
    if (m_notifies.find(wc) == m_notifies.end()) {
        m_notifies.insert({ wc, {
            wc->DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1, _2)),
            wc->DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1, _2))
        } });
    }
}

void Shader::RemoveNotify(std::shared_ptr<WindowContext>& wc)
{
    auto itr = m_notifies.find(wc);
    if (itr != m_notifies.end())
    {
        itr->second.view.disconnect();
        itr->second.proj.disconnect();
        m_notifies.erase(wc);
    }
}

void Shader::ClearNotifies()
{
    for (auto& n : m_notifies) {
        n.second.view.disconnect();
        n.second.proj.disconnect();
    }
    m_notifies.clear();
}

}