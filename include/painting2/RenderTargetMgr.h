//#pragma once
//
//#include <cu/cu_stl.h>
//
//#include <memory>
//
//namespace pt2
//{
//
//class RenderTarget;
//
//class RenderTargetMgr
//{
//public:
//	std::shared_ptr<RenderTarget> Fetch();
//	void Return(std::shared_ptr<RenderTarget>& rt);
//
//	int GetTexID(int idx) const;
//
//	void InitScreenCB(RenderTarget* (*fetch_screen)(), void (*return_screen)(RenderTarget* rt));
//	std::shared_ptr<RenderTarget> FetchScreen();
//	void ReturnScreen(std::shared_ptr<RenderTarget>& rt);
//
//public:
//	static const int WIDTH;
//	static const int HEIGHT;
//
//private:
//	struct Item
//	{
//		std::shared_ptr<RenderTarget> rt;
//		bool available;
//
//		Item() : rt(nullptr), available(true) {}
//	};
//
//private:
//	CU_VEC<Item> m_items;
//
//}; // RenderTargetMgr
//
//}