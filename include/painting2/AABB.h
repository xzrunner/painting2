#pragma once

#include "painting2/BoundingBox.h"

#include <memory>

namespace pt2
{

class AABB : public BoundingBox
{
public:
	virtual BoundingBox* Clone() const override;
	virtual std::unique_ptr<BoundingBox> Clone2() const override;

	virtual size_t Size() const override { return sizeof(AABB); }

	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsContain(const sm::rect& rect) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;

	virtual void CombineTo(sm::rect& r) const override;

	virtual void Build(const sm::rect& r, const sm::vec2& pos = sm::vec2(0, 0), float angle = 0,
		const sm::vec2& scale = sm::vec2(1, 1), const sm::vec2& shear = sm::vec2(0, 0), const sm::vec2& offset = sm::vec2(0, 0)) override;
	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle) override;

	virtual void GetBoundPos(CU_VEC<sm::vec2>& bound) const override;

private:
	sm::rect m_rect;
	sm::vec2 m_position;

}; // AABB

}

#include "painting2/AABB.inl"