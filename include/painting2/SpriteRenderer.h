#pragma once

#include <SM_Matrix.h>

#include <cstdint>
#include <memory>

namespace pt2
{

class Shader;

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Draw(const std::shared_ptr<Shader>& shader, const sm::mat4& mat) const;
	void Draw(unsigned int tex_id, const sm::mat4& mat) const;

private:
	void InitDefaultShader();
	void InitRenderData();

	void FlushShaderlabStatus() const;

private:
	std::shared_ptr<Shader> m_default_shader = nullptr;

	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;

}; // SpriteRenderer

}