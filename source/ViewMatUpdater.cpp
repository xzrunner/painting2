#include "painting2/ViewMatUpdater.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/Uniform.h>
#include <unirender/ShaderProgram.h>

namespace pt2
{

ViewMatUpdater::ViewMatUpdater(const ur::ShaderProgram& shader,
                               const std::string& name, sm::mat4* mat)
    : m_ret_mat(mat)
{
    m_uniform = shader.QueryUniform(name);

    m_view_offset.MakeInvalid();
}

void ViewMatUpdater::Update(const ur::Context& ctx, const ur::DrawState& draw,
                            const void* scene)
{
    if (scene) {
        auto& wc = *static_cast<const WindowContext*>(scene);
        Update(wc.GetViewOffset(), wc.GetViewScale());
    }
}

void ViewMatUpdater::Update(const sm::vec2& offset, float scale)
{
    // for vulkan
    //sm::vec2 revert_y_offset(offset.x, -offset.y);
    sm::vec2 revert_y_offset(offset.x, offset.y);

    if (revert_y_offset == m_view_offset &&
        scale == m_view_scale) {
        return;
    }

    m_view_offset = revert_y_offset;
    m_view_scale  = scale;

    auto s_mat = sm::mat4::Scaled(m_view_scale, m_view_scale, 1);
    auto t_mat = sm::mat4::Translated(m_view_offset.x, m_view_offset.y, 1);
    auto mat = s_mat * t_mat;

    if (m_uniform) {
        m_uniform->SetValue(mat.x, 4 * 4);
    }

    if (m_ret_mat) {
        *m_ret_mat = mat;
    }
}

}