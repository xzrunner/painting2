#include "painting2/Shader.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>

namespace pt2
{

Shader::Shader(WindowContext& wc, ur::RenderContext* rc, const pt0::Shader::Params& params)
	: pt0::Shader(rc, params)
{
	m_conn_view = wc.DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1, _2));
	m_conn_proj = wc.DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1, _2));
}

Shader::~Shader()
{
	m_conn_view.disconnect();
	m_conn_proj.disconnect();
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
	SetMat4(m_uniform_names.view_mat.c_str(), mat.x);
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
	SetMat4(m_uniform_names.proj_mat.c_str(), mat.x);
}

}