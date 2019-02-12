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
    for (auto& n : m_notifies) {
        n.first.disconnect();
        n.second.disconnect();
    }
}

void Shader::UpdateViewMat(const sm::vec2& offset, float scale)
{
	if (offset == m_offset && scale == m_scale) {
		return;
	}
	m_offset = offset;
	m_scale = scale;

	Use();

	auto mat = sm::mat4::Scaled(scale, scale, 1);
	mat.Translate(offset.x * scale, offset.y * scale, 0);

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

void Shader::AddNotify(WindowContext& wc)
{
    m_notifies.push_back({
        wc.DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1, _2)),
        wc.DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1, _2))
    });
}

}