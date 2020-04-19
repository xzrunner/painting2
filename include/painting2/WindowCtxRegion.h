#pragma once

#include <memory>

namespace ur2 { class Context; }

namespace pt2
{

class WindowContext;

class WindowCtxRegion
{
public:
	WindowCtxRegion(const ur2::Context& ctx, float width, float height);
	~WindowCtxRegion();

private:
    const ur2::Context& m_ctx;

	std::shared_ptr<WindowContext> m_old;

}; // WindowCtxRegion

}