#pragma once

#include <SM_Vector.h>
#include <unirender/UniformUpdater.h>

#include <memory>
#include <string>

namespace ur { class ShaderProgram; class Uniform; }

namespace pt2
{

class ViewMatUpdater : public ur::UniformUpdater
{
public:
    ViewMatUpdater(const ur::ShaderProgram& shader,
        const std::string& name);

    virtual ur::UpdaterID UpdaterTypeID() const override {
        return ur::GetUpdaterTypeID<ViewMatUpdater>();
    }

    virtual void Update(const ur::Context& ctx,
        const ur::DrawState& draw, const void* scene = nullptr) override;

    void Update(const sm::vec2& offset, float scale);

public:
    class Snapshot
    {
    public:
        Snapshot(ViewMatUpdater& updater)
            : m_updater(updater), m_offset(updater.m_view_offset), m_scale(updater.m_view_scale) {}

        void Restore() {
            m_updater.Update(m_offset, m_scale);
        }

    private:
        ViewMatUpdater& m_updater;

        sm::vec2 m_offset;
        float    m_scale = 0;

    }; // Snapshot

private:
    std::shared_ptr<ur::Uniform> m_uniform = nullptr;

    sm::vec2 m_view_offset;
    float    m_view_scale = 0;

    friend class Snapshot;

}; // ViewMatUpdater

}