//#include "painting2/RenderScreen.h"
//#include "painting2/Blackboard.h"
//#include "painting2/RenderContext.h"
//#include "painting2/WindowContext.h"
//
//#include <unirender/RenderContext.h>
//#include <unirender/Blackboard.h>
//
//namespace pt2
//{
//
//void RenderScreen::Scissor(float x, float y, float w, float h)
//{
//	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
//	if (!wc) {
//		auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//		ur_rc.SetScissor(0, 0, 0, 0);
//		return;
//	}
//
//	float mv_scale = wc->GetViewScale();
//	x *= mv_scale;
//	y *= mv_scale;
//	w *= mv_scale;
//	h *= mv_scale;
//
//	float screen_w = static_cast<float>(wc->GetScreenWidth()),
//		  screen_h = static_cast<float>(wc->GetScreenHeight());
//	x += screen_w * 0.5f;
//	y += screen_h * 0.5f;
//
//	x += wc->GetViewOffset().x * mv_scale;
//	y += wc->GetViewOffset().y * mv_scale;
//
//	if (x < 0) {
//		w += x;
//		x = 0;
//	} else if (x > screen_w) {
//		w = h = 0;
//	}
//	if (y < 0) {
//		h += y;
//		y = 0;
//	} else if (y > screen_h) {
//		w = h = 0;
//	}
//
//	if (w < 0 || h < 0) {
//		w = h = 0;
//	}
//
//	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//	ur_rc.SetScissor(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
//}
//
//}