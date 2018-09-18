#include "painting2/SpriteRenderer.h"
#include "painting2/Texture.h"
#include "painting2/Blackboard.h"
#include "painting2/WindowContext.h"
#include "painting2/Shader.h"

#include <SM_Matrix.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/SampleTex2D.h>

namespace pt2
{

SpriteRenderer::SpriteRenderer()
{
	InitRenderData();
	InitDefaultShader();
}

SpriteRenderer::~SpriteRenderer()
{
	ur::Blackboard::Instance()->GetRenderContext().ReleaseVAO(m_vao, m_vbo, m_ebo);
}

void SpriteRenderer::Draw(const std::shared_ptr<Shader>& shader, const sm::mat4& mat) const
{
	FlushShaderlabStatus();

	shader->Use();

	shader->SetMat4(shader->GetModelUniformName().c_str(), mat.x);

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.DrawElementsVAO(ur::DRAW_TRIANGLES, 0, 6, m_vao);
}

void SpriteRenderer::Draw(unsigned int tex_id, const sm::mat4& mat) const
{
	FlushShaderlabStatus();

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	m_default_shader->Use();
	rc.BindTexture(tex_id, 0);

	m_default_shader->SetMat4(m_default_shader->GetModelUniformName().c_str(), mat.x);

	rc.DrawElementsVAO(ur::DRAW_TRIANGLES, 0, 6, m_vao);
}

void SpriteRenderer::InitDefaultShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib("position", 2, sizeof(float), 16, 0));
	layout.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float), 16, 8));
	auto layout_id = rc.CreateVertexLayout(layout);
	rc.BindVertexLayout(layout_id);

	// vert
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>("u_view",       sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>("u_model",      sw::t_mat4);

	auto position   = std::make_shared<sw::node::Input>  ("position",     sw::t_pos2);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(2);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::IN_PROJ });
	sw::make_connecting({ view,       0 }, { pos_trans, sw::node::PositionTrans::IN_VIEW });
	sw::make_connecting({ model,      0 }, { pos_trans, sw::node::PositionTrans::IN_MODEL });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::IN_POS });
	vert_nodes.push_back(pos_trans);

	// varying
	auto vert_in_uv  = std::make_shared<sw::node::Input>("texcoord", sw::t_uv);
	auto vert_out_uv = std::make_shared<sw::node::Output>("v_texcoord", sw::t_uv);
	sw::make_connecting({ vert_in_uv, 0 }, { vert_out_uv, 0 });
	vert_nodes.push_back(vert_out_uv);

	// frag
	auto tex_sample = std::make_shared<sw::node::SampleTex2D>();
	auto frag_in_tex = std::make_shared<sw::node::Uniform>("u_texture0", sw::t_tex2d);
	auto frag_in_uv = std::make_shared<sw::node::Input>("v_texcoord", sw::t_uv);
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::IN_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::IN_UV });

	// end
	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ tex_sample }, sw::ST_FRAG);

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	pt2::Shader::Params sp(texture_names, layout);
	sp.vs = vert.GetShaderStr().c_str();
	sp.fs = frag.GetShaderStr().c_str();
	sp.uniform_names.model_mat = "u_model";
	sp.uniform_names.view_mat  = "u_view";
	sp.uniform_names.proj_mat  = "u_projection";
	auto& wc = Blackboard::Instance()->GetWindowContext();
	m_default_shader = std::make_shared<pt2::Shader>(*wc, &rc, sp);
}

void SpriteRenderer::InitRenderData()
{
	ur::RenderContext::VertexInfo vi;

	float vertices[] = {
		// pos          // tex
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