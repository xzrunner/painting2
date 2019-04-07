#include "painting2/RenderTargetCtx.h"
#include "painting2/Shader.h"

#include "painting2/Blackboard.h"
#include "painting2/WindowContext.h"

#include <unirender/RenderContext.h>

namespace pt2
{

RenderTargetCtx::RenderTargetCtx(ur::RenderContext& rc, const std::shared_ptr<Shader>& shader,
                                 int width, int height)
    : m_rc(rc)
    , m_shader(shader)
{
    m_rc.GetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
    m_rc.SetViewport(0, 0, width, height);

    if (m_shader) {
        m_shader->UpdateModelMat(sm::mat4());
        m_shader->UpdateProjMat(width, height);
        m_shader->UpdateViewMat(sm::vec2(), 1);
    }
}

RenderTargetCtx::~RenderTargetCtx()
{
    if (m_shader) {
        // fixme
        auto wc = Blackboard::Instance()->GetWindowContext();
        wc->Bind();
//        m_shader->UpdateProjMat(m_vp_w, m_vp_h);
    }
    m_rc.SetViewport(m_vp_x, m_vp_y, m_vp_w, m_vp_h);
}

}