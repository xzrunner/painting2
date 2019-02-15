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

    void AddNotify(std::shared_ptr<WindowContext>& wc);
    void ClearNotifies();

private:
    struct Notify
    {
        boost::signals2::connection view;
        boost::signals2::connection proj;
    };

private:
	// view
	sm::vec2 m_offset;
	float    m_scale = 0;
	// proj
	int m_width = 0;
	int m_height = 0;

    std::map<std::shared_ptr<WindowContext>, Notify> m_notifies;

    RTTR_ENABLE(pt0::Shader)

}; // Shader

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif