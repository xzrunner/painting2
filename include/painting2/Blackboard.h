#pragma once

#include <guard/check.h>
#include <cu/cu_macro.h>

#include <memory>

namespace pt2
{

class RenderContext;
class ShaderMgr;

class Blackboard
{
public:
	void SetContext(const std::shared_ptr<RenderContext>& ctx) { m_ctx = ctx; }
	RenderContext& GetContext() {
		GD_ASSERT(m_ctx, "null ctx");
		return *m_ctx;
	}

private:
	std::shared_ptr<RenderContext> m_ctx = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}