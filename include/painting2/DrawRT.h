#pragma once

#include <SM_Matrix2D.h>

#include <memory>
#include <functional>

#include <boost/noncopyable.hpp>

namespace pt2
{

class RenderTarget;

class DrawRT : boost::noncopyable
{
public:
	DrawRT();
	~DrawRT();

	template<typename Type>
	void Draw(const Type& obj, std::function<void(const Type&, const sm::Matrix2D&)> draw_func,
		bool whitebg = false, float scale_x = 1.0f, float scale_y = 1.0f) const;

	uint8_t* StoreToMemory(int width = -1, int height = -1, int channels = 4);
	void StoreToFile(const std::string& filepath, int width = -1, int height = -1, int channels = 4);

private:
	std::shared_ptr<RenderTarget> m_rt = nullptr;

}; // DrawRT

}

#include "painting2/DrawRT.inl"