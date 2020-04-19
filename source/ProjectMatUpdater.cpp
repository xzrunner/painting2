#include "painting2/ProjectMatUpdater.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender2/Uniform.h>
#include <unirender2/ShaderProgram.h>

namespace pt2
{

ProjectMatUpdater::ProjectMatUpdater(const ur2::ShaderProgram& shader,
                                     const std::string& name)
{
    m_uniform = shader.QueryUniform(name);
}

void ProjectMatUpdater::Update(const ur2::Context& ctx, const ur2::DrawState& draw,
                               const void* scene)
{
    if (scene) {
        auto& wc = *static_cast<const WindowContext*>(scene);
        Update(wc.GetProjWidth(), wc.GetProjHeight());
    }
}

void ProjectMatUpdater::Update(float width, float height)
{
    if (!m_uniform) {
        return;
    }

    if (width == m_width &&
        height == m_height) {
        return;
    }

    m_width  = width;
    m_height = height;

    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    auto mat = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);
    m_uniform->SetValue(mat.x, 4 * 4);
}

}