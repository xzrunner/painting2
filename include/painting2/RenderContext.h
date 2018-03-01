#pragma once

#include "painting2/RenderScissor.h"
#include "painting2/CameraMgr.h"
#include "painting2/RenderTargetMgr.h"

namespace pt2
{

class RenderContext
{
public:
	RenderScissor& GetScissor() { return m_scissor; }

	CameraMgr& GetCamMgr() { return m_cam_mgr; }

	RenderTargetMgr& GetRTMgr() { return m_rt_mgr; }

private:
	RenderScissor m_scissor;

	CameraMgr m_cam_mgr;

	RenderTargetMgr m_rt_mgr;

}; // RenderContext

}