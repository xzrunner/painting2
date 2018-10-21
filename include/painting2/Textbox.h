#pragma once

#include "painting2/GradientColor.h"

namespace pt2
{

class Textbox
{
public:
	enum HoriAlign
	{
		HA_LEFT		= 0,
		HA_RIGHT	= 1,
		HA_CENTER	= 2,
		HA_AUTO		= 3
	};

	enum VertAlign
	{
		VA_TOP		= 0,
		VA_BOTTOM	= 1,
		VA_CENTER	= 2,
		VA_AUTO		= 3
	};

	enum OverLabel
	{
		OL_OVERFLOW = 0,
		OL_CUT_OFF  = 1,
		OL_CONDENSE = 2,
	};

public:
	int           width;
	int           height;

	int           font_type;
	int           font_size;
	GradientColor font_color;

	bool          has_edge;
	float         edge_size;
	GradientColor edge_color;

	HoriAlign     align_hori;
	VertAlign     align_vert;

	float         space_hori;
	float         space_vert;

	OverLabel     overlabel;

	bool          richtext;

public:
	Textbox()
		: width(100)
		, height(20)

		, font_type(0)
		, font_size(16)

		, has_edge(false)
		, edge_size(1)

		, align_hori(HA_LEFT)
		, align_vert(VA_TOP)

		, space_hori(1)
		, space_vert(1)

		, overlabel(OL_OVERFLOW)

		, richtext(true)
	{
		font_color.items[0].col.FromRGBA(0xffffffff);
		edge_color.items[0].col.FromRGBA(0);
	}

	bool operator == (const Textbox& tb) const
	{
		return
			width		== tb.width &&
			height		== tb.height &&

			font_type	== tb.font_type &&
			font_size	== tb.font_size &&
			font_color	== tb.font_color &&

			has_edge	== tb.has_edge &&
			edge_size	== tb.edge_size &&
			edge_color	== tb.edge_color &&

			align_hori	== tb.align_hori &&
			align_vert	== tb.align_vert &&

			space_hori	== tb.space_hori &&
			space_vert	== tb.space_vert &&

			overlabel   == tb.overlabel &&

			richtext	== tb.richtext;
	}

}; // Textbox

}