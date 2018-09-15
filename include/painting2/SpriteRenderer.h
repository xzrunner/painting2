#pragma once

#include <SM_Matrix.h>

#include <cstdint>
#include <memory>

namespace ur { class Shader; }

namespace pt2
{

class Texture;

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Draw(const std::shared_ptr<ur::Shader>& shader,
		const Texture& tex, const sm::mat4& mat);

private:
	void InitRenderData();

	void FlushShaderlabStatus() const;

private:
	uint32_t m_vao;
	uint32_t m_vbo;
	uint32_t m_ebo;

}; // SpriteRenderer

}