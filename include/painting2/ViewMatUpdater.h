#pragma once

#include <SM_Vector.h>
#include <unirender2/UniformUpdater.h>

#include <memory>
#include <string>

namespace ur2 { class ShaderProgram; class Uniform; }

namespace pt2
{

class ViewMatUpdater : public ur2::UniformUpdater
{
public:
    ViewMatUpdater(const ur2::ShaderProgram& shader,
        const std::string& name);

    virtual ur2::UpdaterID UpdaterTypeID() const override {
        return ur2::GetUpdaterTypeID<ViewMatUpdater>();
    }

    virtual void Update(const ur2::Context& ctx,
        const ur2::DrawState& draw, const void* scene = nullptr) override;

    void Update(const sm::vec2& offset, float scale);

private:
    std::shared_ptr<ur2::Uniform> m_uniform = nullptr;

    sm::vec2 m_view_offset;
    float    m_view_scale = 0;

}; // ViewMatUpdater

}