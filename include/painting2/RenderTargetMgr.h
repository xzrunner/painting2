#pragma once

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>
#include <unirender/typedef.h>

#include <memory>

namespace ur { class Device; class Framebuffer; }

namespace pt2
{

class RenderTargetMgr
{
public:
	std::shared_ptr<ur::Framebuffer> Fetch(const ur::Device& dev);
	void Return(std::shared_ptr<ur::Framebuffer>& fbo);

    ur::TexturePtr GetBindedTex(const std::shared_ptr<ur::Framebuffer>& fbo) const;

	//void InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt));
	//std::shared_ptr<RenderTarget> FetchScreen();
	//void ReturnScreen(std::shared_ptr<RenderTarget>& rt);

public:
	static const int WIDTH;
	static const int HEIGHT;

private:
	struct Item
	{
		std::shared_ptr<ur::Framebuffer> fbo = nullptr;
        ur::TexturePtr                   tex = nullptr;
		bool available;

		Item() : fbo(nullptr), tex(nullptr), available(true) {}
	};

private:
	CU_VEC<Item> m_items;

    CU_SINGLETON_DECLARATION(RenderTargetMgr);

}; // RenderTargetMgr

}