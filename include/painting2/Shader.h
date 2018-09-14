#pragma once

#include <SM_Vector.h>
#include <unirender/Shader.h>

#include <boost/signals2.hpp>

namespace pt2
{

class WindowContext;

class Shader : public ur::Shader
{
public:
	Shader(WindowContext& wc, ur::RenderContext* rc, const char* vs, const char* fs,
		const std::vector<std::string>& textures, const CU_VEC<ur::VertexAttrib>& va_list,
		const std::string& view_name, const std::string& proj_name);
	virtual ~Shader();

private:
	void UpdateViewMat(const sm::vec2& offset, float scale);
	void UpdateProjMat(int width, int height);

private:
	std::string m_view_name;
	std::string m_proj_name;

	// view
	sm::vec2 m_offset;
	float    m_scale = 1;
	// proj
	int m_width = 0;
	int m_height = 0;

	boost::signals2::connection m_conn_view;
	boost::signals2::connection m_conn_proj;

}; // Shader

}