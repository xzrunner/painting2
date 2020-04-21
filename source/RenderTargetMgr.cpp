#include "painting2/RenderTargetMgr.h"

#include <unirender2/Device.h>
#include <unirender2/TextureDescription.h>
#include <unirender2/Framebuffer.h>

#include <guard/check.h>

namespace pt2
{

const int RenderTargetMgr::WIDTH  = 1024;
const int RenderTargetMgr::HEIGHT = 1024;

//static RenderTarget* (*FETCH_SCREEN)() = nullptr;
//static void (*RETURN_SCREEN)(RenderTarget* rt) = nullptr;

CU_SINGLETON_DEFINITION(RenderTargetMgr);

RenderTargetMgr::RenderTargetMgr()
{
}

std::shared_ptr<ur2::Framebuffer> RenderTargetMgr::Fetch(const ur2::Device& dev)
{
	for (int i = 0, n = m_items.size(); i < n; ++i)
	{
		if (!m_items[i].available) {
			continue;
		}
		m_items[i].available = false;
		GD_ASSERT(m_items[i].fbo, "null rt");
		return m_items[i].fbo;
	}

    ur2::TextureDescription desc;
    desc.width  = WIDTH;
    desc.height = HEIGHT;
    desc.target = ur2::TextureTarget::Texture2D;
    desc.format = ur2::TextureFormat::RGBA8;
    auto tex = dev.CreateTexture(desc);

    auto fbo = dev.CreateFramebuffer();

    const auto type = ur2::AttachmentType::Color0;
    fbo->SetAttachment(type, ur2::TextureTarget::Texture2D, tex, nullptr);

	Item item;
	item.fbo = fbo;
    item.tex = tex;
	item.available = false;
	m_items.push_back(item);

	return item.fbo;
}

void RenderTargetMgr::Return(std::shared_ptr<ur2::Framebuffer>& fbo)
{
	if (!fbo) {
		return;
	}

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i].fbo == fbo) {
			m_items[i].available = true;
			return;
		}
	}
}

ur2::TexturePtr RenderTargetMgr::GetBindedTex(const std::shared_ptr<ur2::Framebuffer>& fbo) const
{
    for (auto& item : m_items) {
        if (item.fbo == fbo) {
            return item.tex;
        }
    }
    return nullptr;
}

//
//int RenderTargetMgr::GetTexID(int idx) const
//{
//	if (idx < 0 || idx >= static_cast<int>(m_items.size())) {
//		return  -1;
//	} else {
//		return m_items[idx].rt->GetTexID();
//	}
//}

//void RenderTargetMgr::InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt))
//{
//	FETCH_SCREEN = fetch_screen;
//	RETURN_SCREEN = return_screen;
//}
//
//std::shared_ptr<RenderTarget> RenderTargetMgr::FetchScreen()
//{
//	if (FETCH_SCREEN) {
//		return FETCH_SCREEN();
//	} else {
//		return nullptr;
//	}
//}
//
//void RenderTargetMgr::ReturnScreen(std::shared_ptr<RenderTarget>& rt)
//{
//	if (RETURN_SCREEN) {
//		RETURN_SCREEN(rt);
//	}
//}

}