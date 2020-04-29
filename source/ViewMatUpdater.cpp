#include "painting2/ViewMatUpdater.h"
#include "painting2/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/Uniform.h>
#include <unirender/ShaderProgram.h>

namespace pt2
{

ViewMatUpdater::ViewMatUpdater(const ur::ShaderProgram& shader,
                               const std::string& name)
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
    if (!m_uniform) {
        return;
    }

    if (offset == m_view_offset &&
        scale == m_view_scale) {
        return;
    }

    m_view_offset = offset;
    m_view_scale  = scale;

    auto s_mat = sm::mat4::Scaled(m_view_scale, m_view_scale, 1);
    auto t_mat = sm::mat4::Translated(m_view_offset.x, m_view_offset.y, 1);
    auto mat = s_mat * t_mat;
    m_uniform->SetValue(mat.x, 4 * 4);
}

}