#include "painting2/WindowCtxRegion.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"
#include "painting2/WindowContext.h"

namespace pt2
{

WindowCtxRegion::WindowCtxRegion(const ur::Context& ctx,
                                 float width, float height)
    : m_ctx(ctx)
{
	//m_old = Blackboard::Instance()->GetWindowContext();

	//auto new_wc = std::make_shared<pt2::WindowContext>(
	//	static_cast<float>(width), static_cast<float>(height), 0, 0);
	//new_wc->Bind(*m_ctx);
	//Blackboard::Instance()->SetWindowContext(new_wc);
}

WindowCtxRegion::~WindowCtxRegion()
{
	//if (m_old) {
	//	m_old->Bind(*m_ctx);
	//}
	//Blackboard::Instance()->SetWindowContext(m_old);
}

}