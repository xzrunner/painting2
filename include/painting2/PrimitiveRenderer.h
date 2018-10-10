#pragma once

#include <SM_Matrix.h>

#include <memory>

namespace pt0 { class Shader; }
namespace pd { class DrawList; }

namespace pt2
{

class PrimitiveRenderer
{
public:
	PrimitiveRenderer();
	~PrimitiveRenderer();

	void Draw(const pd::DrawList& dlist,
		unsigned int tex_id, const sm::mat4& mat) const;

private:
	void InitDefaultShader();

private:
	std::shared_ptr<pt0::Shader> m_default_shader = nullptr;

	uint32_t m_vbo = 0, m_ebo = 0;

}; // PrimitiveRenderer

}