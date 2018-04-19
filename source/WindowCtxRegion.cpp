#include "painting2/WindowCtxRegion.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"
#include "painting2/WindowContext.h"

namespace pt2
{

WindowCtxRegion::WindowCtxRegion(float width, float height)
{
	m_old = Blackboard::Instance()->GetWindowContext();

	auto& rt_mgr = Blackboard::Instance()->GetRenderContext().GetRTMgr();
	auto new_wc = std::make_shared<pt2::WindowContext>(
		static_cast<float>(rt_mgr.WIDTH), static_cast<float>(rt_mgr.HEIGHT), rt_mgr.WIDTH, rt_mgr.HEIGHT);
	new_wc->Bind();
	Blackboard::Instance()->SetWindowContext(new_wc);
}

WindowCtxRegion::~WindowCtxRegion()
{
	m_old->Bind();
	Blackboard::Instance()->SetWindowContext(m_old);
}

}