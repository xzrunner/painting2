#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace pt2
{

class RenderContext;
class WindowContext;

class Blackboard
{
public:
	//void SetRenderContext(const std::shared_ptr<RenderContext>& rc) { m_rc = rc; }
	//RenderContext& GetRenderContext();

	//void SetWindowContext(const std::shared_ptr<WindowContext>& wc) { m_wc = wc; }
	//const std::shared_ptr<WindowContext>& GetWindowContext() { return m_wc; }

private:
	std::shared_ptr<RenderContext> m_rc = nullptr;
	std::shared_ptr<WindowContext> m_wc = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}