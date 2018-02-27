#pragma once

#include "painting2/RenderContext.h"

#include <cu/cu_stl.h>

namespace pt2
{

class RenderCtxStack
{
public:
	int  Push(const RenderContext& ctx);
	void Pop();

	const RenderContext* Top() const;

	int Size() const { return m_stack.size(); }

	// todo
	bool Bind(int idx);
	
private:
	static void BindCtx(const RenderContext& ctx);

private:
	CU_VEC<RenderContext> m_stack;

}; // RenderCtxStack

}