#include "painting2/WndCtxStack.h"

namespace pt2
{

int WndCtxStack::Push(const WindowContext& wx)
{
	int idx = m_stack.size();
	BindCtx(wx);
	m_stack.push_back(wx);
	return idx;
}

void WndCtxStack::Pop()
{
	if (m_stack.empty()) {
		return;
	}

	m_stack.pop_back();

	if (!m_stack.empty()) {
		BindCtx(m_stack.back());		
	}
}

const WindowContext* WndCtxStack::Top() const
{
	if (m_stack.empty()) {
		return nullptr;
	} else {
		return &m_stack[m_stack.size() - 1];
	}
}

bool WndCtxStack::Bind(int idx)
{
	if (idx < 0 || idx > static_cast<int>(m_stack.size())) {
		return false;
	}
	BindCtx(m_stack[idx]);
	return true;
}

void WndCtxStack::BindCtx(const WindowContext& wx)
{
	wx.UpdateMVP();
	wx.UpdateViewport();
}

}