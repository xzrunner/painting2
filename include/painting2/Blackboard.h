#pragma once

#include <guard/check.h>
#include <cu/cu_macro.h>

#include <memory>

namespace pt2
{

class Context;
class ShaderMgr;

class Blackboard
{
public:
	void SetContext(const std::shared_ptr<Context>& ctx) { m_ctx = ctx; }
	Context& GetContext() {
		GD_ASSERT(m_ctx, "null ctx");
		return *m_ctx;
	}

private:
	std::shared_ptr<Context> m_ctx = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}