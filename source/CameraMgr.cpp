#include "painting2/CameraMgr.h"

namespace pt2
{

CU_SINGLETON_DEFINITION(CameraMgr)

CameraMgr::CameraMgr() 
	: m_type(ORTHO)
{
}

CameraMgr::~CameraMgr() 
{
}

}