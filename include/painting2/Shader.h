#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <SM_Vector.h>
#include <unirender/Shader.h>

#include <boost/signals2.hpp>

namespace pt2
{

class WindowContext;

class Shader : public ur::Shader
{
public:
	struct ShaderParams
	{
		ShaderParams(const std::vector<std::string>& textures,
			const std::vector<ur::VertexAttrib>& va_list)
			: textures(textures), va_list(va_list) {}

		const char* vs = nullptr;
		const char* fs = nullptr;
		const std::vector<std::string>& textures;
		const std::vector<ur::VertexAttrib>& va_list;

		// uniform matrix
		std::string model_name;
		std::string view_name;
		std::string proj_name;
	};

public:
	Shader(WindowContext& wc, ur::RenderContext* rc, const ShaderParams& sp);
	virtual ~Shader();

	auto& GetModelUniformName() const { return m_model_name; }

private:
	void UpdateViewMat(const sm::vec2& offset, float scale);
	void UpdateProjMat(int width, int height);

private:
	// uniform matrix
	std::string m_model_name;
	std::string m_view_name;
	std::string m_proj_name;

	// view
	sm::vec2 m_offset;
	float    m_scale = 0;
	// proj
	int m_width = 0;
	int m_height = 0;

	boost::signals2::connection m_conn_view;
	boost::signals2::connection m_conn_proj;

}; // Shader

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif