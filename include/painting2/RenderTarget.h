#pragma once

namespace ur { class RenderTarget; }

namespace pt2
{

class RenderTarget
{
public:
	RenderTarget(int width, int height, bool has_depth = false);
	virtual ~RenderTarget();

	virtual void Bind();
	virtual void Unbind();

	int Width() const;
	int Height() const;

	int GetTexID() const;

	void Resize(int width, int height);

private:
	ur::RenderTarget* m_impl;

}; // RenderTarget

}