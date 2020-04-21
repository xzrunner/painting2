#pragma once

#include <cu/cu_stl.h>
#include <cu/cu_macro.h>
#include <unirender2/typedef.h>

#include <memory>

namespace ur2 { class Device; class Framebuffer; }

namespace pt2
{

class RenderTargetMgr
{
public:
	std::shared_ptr<ur2::Framebuffer> Fetch(const ur2::Device& dev);
	void Return(std::shared_ptr<ur2::Framebuffer>& fbo);

    ur2::TexturePtr GetBindedTex(const std::shared_ptr<ur2::Framebuffer>& fbo) const;

	//void InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt));
	//std::shared_ptr<RenderTarget> FetchScreen();
	//void ReturnScreen(std::shared_ptr<RenderTarget>& rt);

public:
	static const int WIDTH;
	static const int HEIGHT;

private:
	struct Item
	{
		std::shared_ptr<ur2::Framebuffer> fbo = nullptr;
        ur2::TexturePtr                   tex = nullptr;
		bool available;

		Item() : fbo(nullptr), tex(nullptr), available(true) {}
	};

private:
	CU_VEC<Item> m_items;

    CU_SINGLETON_DECLARATION(RenderTargetMgr);

}; // RenderTargetMgr

}