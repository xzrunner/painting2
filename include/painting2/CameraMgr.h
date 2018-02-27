#pragma once

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

	CameraMgr();

public:
	bool IsType(Type t) const { return m_type == t; }

private:
	Type m_type;

}; // CameraMgr

}