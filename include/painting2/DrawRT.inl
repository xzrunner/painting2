//#pragma once
//
//#include "painting2/Blackboard.h"
//#include "painting2/RenderContext.h"
//#include "painting2/RenderTargetMgr.h"
//#include "painting2/RenderTarget.h"
//#include "painting2/RenderContext.h"
//#include "painting2/WindowCtxRegion.h"
//#include "painting2/Callback.h"
//
//#include <renderpipeline/RenderMgr.h>
//
//namespace pt2
//{
//
//template<typename Type>
//inline void DrawRT::Draw(const Type& obj, std::function<void(const Type&, const sm::Matrix2D&)> draw_func,
//	                     bool whitebg, float scale_x, float scale_y) const
//{
//	m_rt->Bind();
//
//	rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SPRITE);
//
//	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//	ur_rc.SetClearFlag(ur::MASKC);
//	uint32_t clear_color;
//	if (whitebg) {
//		clear_color = 0xffffffff;
//	} else {
//		clear_color = 0;
//	}
//	ur_rc.SetClearColor(clear_color);
//	ur_rc.Clear();
//
//	auto rect = Callback::GetBounding(obj);
//	auto sz = rect.Size();
//	int w = static_cast<int>(sz.x * scale_x),
//		h = static_cast<int>(sz.y * scale_y);
//
//	auto& rc = Blackboard::Instance()->GetRenderContext();
//	rc.GetScissor().Disable();
//	{
//		WindowCtxRegion wcr(static_cast<float>(w), static_cast<float>(h));
//
//		sm::Matrix2D mt;
//		auto center = rect.Center();
//		mt.SetTransformation(-center.x, center.y, 0, 1, -1, 0, 0, 0, 0);
//		draw_func(obj, mt);
//	}
//	rc.GetScissor().Enable();
//
//	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
//	rp::RenderMgr::Instance()->Flush();
//
//	m_rt->Unbind();
//}
//
//}