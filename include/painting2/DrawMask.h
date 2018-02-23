#pragma once

#include "painting2/RenderReturn.h"

#include <boost/noncopyable.hpp>

namespace cooking { class DisplayList; }

namespace pt2
{

class RenderTarget;

template<typename Type, typename Params>
class DrawMask : boost::noncopyable
{
public:
	DrawMask(const Type& base, const Type& mask, const Params& params);

	virtual RenderReturn Draw(cooking::DisplayList* dlist) const = 0;

//	static RenderReturn DrawByStencil(const Sprite* base, const Sprite* mask, const RenderParams& rp);

protected:
	RenderReturn DrawImpl(cooking::DisplayList* dlist) const;

	virtual RenderReturn DrawBaseNode(const Type& data, const Params& params) const = 0;
	virtual RenderReturn DrawMaskNode(const Type& data, const Params& params) const = 0;

	virtual sm::rect GetBounding(const Type& data) const = 0;

	virtual const sm::Matrix2D& GetMat(const Params& params) const = 0;
	
private:
	RenderReturn DrawBaseToRT(cooking::DisplayList* dlist, RenderTarget* rt) const;
	RenderReturn DrawMaskToRT(cooking::DisplayList* dlist, RenderTarget* rt) const;

	RenderReturn DrawMaskFromRT(cooking::DisplayList* dlist, RenderTarget* rt_base, RenderTarget* rt_mask) const;

protected:
	const Type&   m_base;
	const Type&   m_mask;
	const Params& m_params;

}; // DrawMask

}

#include "painting2/DrawMask.inl"