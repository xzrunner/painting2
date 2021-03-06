#pragma once

#include "painting2/RenderFilter.h"
#include "painting2/FilterMode.h"

#include <cu/cu_macro.h>

#include <memory>

namespace pt2
{

class FilterFactory
{
public:
	RenderFilterPtr Create(FilterMode mode);

	RenderFilter* GetTemp(FilterMode mode) {
		return m_temps[mode];
	}

private:
	static const int MAX_COUNT = 100;

private:
	RenderFilter* m_temps[MAX_COUNT];

private:
	CU_SINGLETON_DECLARATION(FilterFactory)

}; // FilterFactory

}