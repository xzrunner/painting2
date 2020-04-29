#pragma once

#include <memory>

namespace ur { class Context; }

namespace pt2
{

class WindowContext;

class WindowCtxRegion
{
public:
	WindowCtxRegion(const ur::Context& ctx, float width, float height);
	~WindowCtxRegion();

private:
    const ur::Context& m_ctx;

	std::shared_ptr<WindowContext> m_old;

}; // WindowCtxRegion

}