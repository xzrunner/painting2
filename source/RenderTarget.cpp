#include "painting2/RenderTarget.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <stat/StatImages.h>

namespace pt2
{

static const int IMG_ID = -2;

RenderTarget::RenderTarget(int width, int height)
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	m_impl = new ur::RenderTarget(&ur_rc, width, height);

	st::StatImages::Instance()->Add(IMG_ID, width, height, ur::TEXTURE_RGBA8);
}

RenderTarget::~RenderTarget()
{
	st::StatImages::Instance()->Remove(IMG_ID, m_impl->Width(), m_impl->Height(), ur::TEXTURE_RGBA8);

	delete m_impl;
}

void RenderTarget::Bind()
{
	m_impl->Bind();
}

void RenderTarget::Unbind()
{
	m_impl->Unbind();
}

int RenderTarget::Width() const
{
	return m_impl->Width();
}

int RenderTarget::Height() const
{
	return m_impl->Height();
}

int RenderTarget::GetTexID() const
{
	return m_impl->TexID();
}

void RenderTarget::Resize(int width, int height)
{
	m_impl->Resize(width, height);
}

}