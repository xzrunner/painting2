#pragma once

#include <memory>

namespace ur { class RenderContext; }

namespace pt2
{

class Shader;

class RenderTargetCtx
{
public:
    RenderTargetCtx(ur::RenderContext& rc, const std::shared_ptr<Shader>& shader,
        int width, int height);
    ~RenderTargetCtx();

private:
    ur::RenderContext& m_rc;
    std::shared_ptr<Shader> m_shader = nullptr;

    int m_vp_x, m_vp_y, m_vp_w, m_vp_h;

}; // RenderTargetCtx

}