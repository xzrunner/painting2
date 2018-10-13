#pragma once

#include <SM_Matrix.h>

#include <memory>

namespace pt0 { class Shader; }
namespace tess { class Painter; class Palette; }

namespace pt2
{

class PrimitiveRenderer
{
public:
	PrimitiveRenderer();
	~PrimitiveRenderer();

	void Draw(const tess::Painter& pt, const sm::mat4& mat) const;

private:
	void InitDefaultShader();

private:
	std::shared_ptr<pt0::Shader> m_default_shader = nullptr;

	std::unique_ptr<tess::Palette> m_palette = nullptr;

	uint32_t m_vbo = 0, m_ebo = 0;

}; // PrimitiveRenderer

}