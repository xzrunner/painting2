#include "painting2/AABB.h"

#include <memmgr/Allocator.h>

namespace pt2
{

BoundingBox* AABB::Clone() const
{
	void* ptr = mm::AllocHelper::Allocate(Size());
	return new (ptr) AABB(*this);
}

std::unique_ptr<BoundingBox> AABB::Clone2() const
{
	return std::make_unique<AABB>(*this);
}

}