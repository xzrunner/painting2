#pragma once

#include <SM_Vector.h>
#include <unirender/UniformUpdater.h>

#include <string>

namespace ur { class ShaderProgram; class Uniform; }

namespace pt2
{

class ProjectMatUpdater : public ur::UniformUpdater
{
public:
    ProjectMatUpdater(const ur::ShaderProgram& shader,
        const std::string& name);

    virtual ur::UpdaterID UpdaterTypeID() const override {
        return ur::GetUpdaterTypeID<ProjectMatUpdater>();
    }

    virtual void Update(const ur::Context& ctx,
        const ur::DrawState& draw, const void* scene = nullptr) override;

    void Update(float width, float height);

public:
    class Snapshot
    {
    public:
        Snapshot(ProjectMatUpdater& updater)
            : m_updater(updater), m_width(updater.m_width), m_height(updater.m_height) {}

        void Restore() {
            m_updater.Update(m_width, m_height);
        }

    private:
        ProjectMatUpdater& m_updater;

        float m_width = 0;
        float m_height = 0;

    }; // Snapshot

private:
    std::shared_ptr<ur::Uniform> m_uniform = nullptr;

    float m_width  = 0;
    float m_height = 0;

    friend class Snapshot;

}; // ProjectMatUpdater

}