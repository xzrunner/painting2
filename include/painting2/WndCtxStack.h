#pragma once

#include "painting2/WindowContext.h"

#include <cu/cu_stl.h>

namespace pt2
{

class WndCtxStack
{
public:
	int  Push(const WindowContext& wc);
	void Pop();

	const WindowContext* Top() const;

	int Size() const { return m_stack.size(); }

	// todo
	bool Bind(int idx);
	
private:
	static void BindCtx(const WindowContext& wc);

private:
	CU_VEC<WindowContext> m_stack;

}; // WndCtxStack

}