#pragma once

#include <cu/cu_macro.h>

namespace pt2
{

class CameraMgr
{
public:
	enum Type
	{
		ORTHO = 0,
		PSEUDO3D,

		MAX_COUNT,
	};

public:
	bool IsType(Type t) const { return m_type == t; }

private:
	Type m_type;

	CU_SINGLETON_DECLARATION(CameraMgr)

}; // CameraMgr

}