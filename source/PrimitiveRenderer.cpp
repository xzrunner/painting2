#include "painting2/PrimitiveRenderer.h"
#include "painting2/Blackboard.h"
#include "painting2/Utility.h"

#include <unirender/Blackboard.h>
#include <unirender/VertexAttrib.h>
#include <unirender/RenderContext.h>
#include <painting2/Shader.h>
#include <shaderweaver/typedef.h>
#include <shaderweaver/Evaluator.h>
#include <shaderweaver/node/Uniform.h>
#include <shaderweaver/node/Input.h>
#include <shaderweaver/node/Output.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/SampleTex2D.h>
#include <primdraw/DrawList.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

#include <shaderweaver/node/Vector4.h>

#include <vector>

namespace pt2
{

PrimitiveRenderer::PrimitiveRenderer()
{
	Utility::FlushShaderlabStatus();

	InitDefaultShader();

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	m_vbo = rc.CreateBuffer(ur::VERTEXBUFFER, nullptr, 0);
	m_ebo = rc.CreateBuffer(ur::INDEXBUFFER, nullptr, 0);
}

PrimitiveRenderer::~PrimitiveRenderer()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.ReleaseBuffer(ur::VERTEXBUFFER, m_vbo);
	rc.ReleaseBuffer(ur::INDEXBUFFER, m_ebo);
}

void PrimitiveRenderer::Draw(const pd::DrawList& dlist,
	                         unsigned int tex_id,
	                         const sm::mat4& mat) const
{
	Utility::FlushShaderlabStatus();

	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	m_default_shader->Use();
	rc.BindTexture(tex_id, 0);

	m_default_shader->SetMat4("u_model", mat.x);

	auto& buf = dlist.GetBuffer();

	rc.BindBuffer(ur::VERTEXBUFFER, m_vbo);
	size_t vbuf_sz = sizeof(pd::DrawList::Vertex) * buf.vertices.size();
	rc.UpdateBuffer(m_vbo, buf.vertices.data(), vbuf_sz);

	rc.BindBuffer(ur::INDEXBUFFER, m_ebo);
	size_t ibuf_sz = sizeof(unsigned short) * buf.indices.size();
	rc.UpdateBuffer(m_ebo, buf.indices.data(), ibuf_sz);

	rc.DrawElements(ur::DRAW_TRIANGLES, 0, buf.indices.size());
}

void PrimitiveRenderer::InitDefaultShader()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	// layout
	std::vector<ur::VertexAttrib> layout;
	layout.push_back(ur::VertexAttrib("position", 2, sizeof(float),    20, 0));
	layout.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float),    20, 8));
	layout.push_back(ur::VertexAttrib("color",    1, sizeof(uint32_t), 20, 16));
	auto layout_id = rc.CreateVertexLayout(layout);
	rc.BindVertexLayout(layout_id);

	// vert
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto view       = std::make_shared<sw::node::Uniform>("u_view",       sw::t_mat4);
	auto model      = std::make_shared<sw::node::Uniform>("u_model",      sw::t_mat4);

	auto position   = std::make_shared<sw::node::Input>  ("position",     sw::t_pos2);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(2);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view,       0 }, { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model,      0 }, { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::ID_POS });
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
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::ID_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::ID_UV });

	auto color_out = std::make_shared<sw::node::Vector4>("zz", sm::vec4(1, 0, 0, 1));

	// end
	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
//	sw::Evaluator frag({ tex_sample }, sw::ST_FRAG);
	sw::Evaluator frag({ color_out }, sw::ST_FRAG);

	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", vert.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");
	//printf("%s\n", frag.GetShaderStr().c_str());
	//printf("//////////////////////////////////////////////////////////////////////////\n");

	std::vector<std::string> texture_names;
	Shader::Params sp(texture_names, layout);
	sp.vs = vert.GetShaderStr().c_str();
	sp.fs = frag.GetShaderStr().c_str();
	sp.uniform_names.model_mat = "u_model";
	sp.uniform_names.view_mat  = "u_view";
	sp.uniform_names.proj_mat  = "u_projection";
	auto& wc = Blackboard::Instance()->GetWindowContext();
	m_default_shader = std::make_shared<Shader>(*wc, &rc, sp);
}

}