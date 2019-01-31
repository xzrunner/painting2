#pragma once
#pragma once

#include <painting0/Color.h>

namespace pt2
{

class RenderColorMap
{
public:
	pt0::Color rmap, gmap, bmap;

public:
	RenderColorMap();
	RenderColorMap(const pt0::Color& rmap, const pt0::Color& gmap, const pt0::Color& bmap);

	RenderColorMap operator * (const RenderColorMap& rc) const;

	bool operator == (const RenderColorMap& rc) const;

	void Reset();

	static void Mul(const RenderColorMap& c0, const RenderColorMap& c1, RenderColorMap& c);

}; // RenderColorMap

}