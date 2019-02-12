#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <painting0/Shader.h>

#include <SM_Vector.h>

#include <boost/signals2.hpp>

namespace pt2
{

class WindowContext;

class Shader : public pt0::Shader
{
public:
	Shader(ur::RenderContext* rc, const pt0::Shader::Params& params);
    virtual ~Shader();

	void UpdateViewMat(const sm::vec2& offset, float scale);
	void UpdateProjMat(int width, int height);

    void AddNotify(WindowContext& wc);

private:
	// view
	sm::vec2 m_offset;
	float    m_scale = 0;
	// proj
	int m_width = 0;
	int m_height = 0;

    std::vector<std::pair<boost::signals2::connection, boost::signals2::connection>> m_notifies;

}; // Shader

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif