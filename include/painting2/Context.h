#pragma once

#include "painting2/RenderCtxStack.h"
#include "painting2/RenderScissor.h"
#include "painting2/CameraMgr.h"
#include "painting2/RenderTargetMgr.h"

namespace pt2
{

class Context
{
public:

	RenderCtxStack& GetCtxStack() { return m_ctx_stack; }

	RenderScissor& GetScissor() { return m_scissor; }

	CameraMgr& GetCamMgr() { return m_cam_mgr; }

	RenderTargetMgr& GetRTMgr() { return m_rt_mgr; }

private:
	RenderCtxStack m_ctx_stack;

	RenderScissor m_scissor;

	CameraMgr m_cam_mgr;

	RenderTargetMgr m_rt_mgr;

}; // Context

}