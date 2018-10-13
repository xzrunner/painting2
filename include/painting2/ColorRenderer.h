#pragma once

#include <SM_Matrix.h>

#include <cstdint>
#include <memory>

namespace pt2
{

class Shader;

class ColorRenderer
{
public:
	ColorRenderer();
	~ColorRenderer();

	void Draw(const std::shared_ptr<Shader>& shader, const sm::mat4& mat);

private:
	void InitRenderData();

private:
	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;

}; // ColorRenderer

}