#pragma once

#include <cu/cu_stl.h>

namespace pt2
{

class RenderTarget;

class RenderTargetMgr
{
public:
	RenderTarget* Fetch();
	void Return(RenderTarget* rt);

	int GetTexID(int idx) const;

	void InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt));
	RenderTarget* FetchScreen();
	void ReturnScreen(RenderTarget* rt);

public:
	static const int WIDTH;
	static const int HEIGHT;

private:
	struct Item
	{
		RenderTarget* rt;
		bool available;

		Item() : rt(nullptr), available(true) {}
	};
	
private:
	CU_VEC<Item> m_items;

}; // RenderTargetMgr

}