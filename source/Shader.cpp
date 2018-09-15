#include "painting2/Shader.h"
#include "painting2/WindowContext.h"

// todo: rm dependence of shaderlab
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

namespace pt2
{

Shader::Shader(WindowContext& wc, ur::RenderContext* rc, const ShaderParams& sp)
	: ur::Shader(rc, sp.vs, sp.fs, sp.textures, sp.va_list)
	, m_model_name(sp.model_name)
	, m_view_name(sp.view_name)
	, m_proj_name(sp.proj_name)
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

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::EXTERN_SHADER);
	shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

	Use();

	auto mat = sm::mat4::Scaled(scale, scale, 1);
	mat.Translate(offset.x * scale, offset.y * scale, 0);
	SetMat4(m_view_name.c_str(), mat.x);
}

void Shader::UpdateProjMat(int width, int height)
{
	if (m_width == width && m_height == height) {
		return;
	}
	m_width = width;
	m_height = height;

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::EXTERN_SHADER);
	shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

	Use();

	float hw = width * 0.5f;
	float hh = height * 0.5f;
	auto mat = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);
	SetMat4(m_proj_name.c_str(), mat.x);
}

}