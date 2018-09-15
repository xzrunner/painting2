#include "painting2/SpriteRenderer.h"
#include "painting2/Texture.h"
#include "painting2/Blackboard.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <unirender/Shader.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

namespace pt2
{

SpriteRenderer::SpriteRenderer()
{
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	ur::Blackboard::Instance()->GetRenderContext().ReleaseVAO(m_vao, m_vbo, m_ebo);
}

void SpriteRenderer::Draw(const std::shared_ptr<ur::Shader>& shader,
	                      const Texture& tex, const sm::mat4& mat)
{
	FlushShaderlabStatus();

	shader->Use();

	shader->SetMat4("u_model", mat.x);

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.BindTexture(tex.TexID(), 0);

	rc.DrawElementsVAO(ur::DRAW_TRIANGLES, 0, 6, m_vao);
}

void SpriteRenderer::InitRenderData()
{
	ur::RenderContext::VertexInfo vi;

	float vertices[] = {
		// pos        // tex
		-0.5f, -0.5f,   0.0f, 0.0f,
		 0.5f, -0.5f,   1.0f, 0.0f,
		 0.5f,  0.5f,   1.0f, 1.0f,
		-0.5f,  0.5f,   0.0f, 1.0f,
	};
	vi.vn = 4;
	vi.vertices = vertices;
	vi.stride = 4 * sizeof(float);

	unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };
	vi.in = 6;
	vi.indices = indices;

	vi.va_list.push_back(ur::VertexAttrib("pos", 2, 4, 16, 0));
	vi.va_list.push_back(ur::VertexAttrib("tex", 2, 4, 16, 8));

	ur::Blackboard::Instance()->GetRenderContext().CreateVAO(vi, m_vao, m_vbo, m_ebo);
}

void SpriteRenderer::FlushShaderlabStatus() const
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::EXTERN_SHADER);
	shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

}

}