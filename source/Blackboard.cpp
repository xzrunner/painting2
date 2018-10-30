#include "painting2/Blackboard.h"

#include <guard/check.h>

namespace pt2
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
{
}

RenderContext& Blackboard::GetRenderContext()
{
	GD_ASSERT(m_rc, "null rc");
	return *m_rc;
}

}