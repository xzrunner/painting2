#pragma once

#include <SM_Vector.h>
#include <unirender2/UniformUpdater.h>

#include <string>

namespace ur2 { class ShaderProgram; class Uniform; }

namespace pt2
{

class ProjectMatUpdater : public ur2::UniformUpdater
{
public:
    ProjectMatUpdater(const ur2::ShaderProgram& shader,
        const std::string& name);

    virtual ur2::UpdaterID UpdaterTypeID() const override {
        return ur2::GetUpdaterTypeID<ProjectMatUpdater>();
    }

    virtual void Update(const ur2::Context& ctx,
        const ur2::DrawState& draw, const void* scene = nullptr) override;

    void Update(float width, float height);

private:
    std::shared_ptr<ur2::Uniform> m_uniform = nullptr;

    float m_width  = 0;
    float m_height = 0;

}; // ProjectMatUpdater

}