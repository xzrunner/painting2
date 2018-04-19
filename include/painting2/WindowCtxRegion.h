#pragma once

#include <memory>

namespace pt2
{

class WindowContext;

class WindowCtxRegion
{
public:
	WindowCtxRegion(float width, float height);
	~WindowCtxRegion();

private:
	std::shared_ptr<WindowContext> m_old;

}; // WindowCtxRegion

}