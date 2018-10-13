#include "painting2/ColorRenderer.h"
#include "painting2/Blackboard.h"
#include "painting2/WindowContext.h"
#include "painting2/Shader.h"
#include "painting2/Utility.h"

#include <SM_Matrix.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace pt2
{

ColorRenderer::ColorRenderer()
{
	InitRenderData();
}

ColorRenderer::~ColorRenderer()
{
	ur::Blackboard::Instance()->GetRenderContext().ReleaseVAO(m_vao, m_vbo, m_ebo);
}

void ColorRenderer::Draw(const std::shared_ptr<Shader>& shader, const sm::mat4& mat)
{
	Utility::FlushShaderlabStatus();

	shader->Use();

	shader->SetMat4(shader->GetModelUniformName().c_str(), mat.x);

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.DrawElementsVAO(ur::DRAW_TRIANGLES, 0, 6, m_vao);
}

void ColorRenderer::InitRenderData()
{
	ur::RenderContext::VertexInfo vi;

	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};
	vi.vn = 4;
	vi.vertices = vertices;
	vi.stride = 4 * sizeof(float);

	unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };
	vi.in = 6;
	vi.indices = indices;

	vi.va_list.push_back(ur::VertexAttrib("pos", 2, 4, 8, 0));

	ur::Blackboard::Instance()->GetRenderContext().CreateVAO(vi, m_vao, m_vbo, m_ebo);
}

}