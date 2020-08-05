#include "painting2/ProjectMatUpdater.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/Uniform.h>
#include <unirender/ShaderProgram.h>

namespace pt2
{

ProjectMatUpdater::ProjectMatUpdater(const ur::ShaderProgram& shader,
                                     const std::string& name, sm::mat4* mat)
    : m_ret_mat(mat)
{
    m_uniform = shader.QueryUniform(name);
}

void ProjectMatUpdater::Update(const ur::Context& ctx, const ur::DrawState& draw,
                               const void* scene)
{
    if (scene) {
        auto& wc = *static_cast<const WindowContext*>(scene);
        Update(wc.GetProjWidth(), wc.GetProjHeight());
    }
}

void ProjectMatUpdater::Update(float width, float height)
{
    if (width == m_width &&
        height == m_height) {
        return;
    }

    m_width  = width;
    m_height = height;

    float hw = m_width * 0.5f;
    float hh = m_height * 0.5f;
    auto mat = sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);

    if (m_uniform) {
        m_uniform->SetValue(mat.x, 4 * 4);
    }

    if (m_ret_mat) {
        *m_ret_mat = mat;
    }
}

}