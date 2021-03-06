#pragma once

#include "painting2/BoundingBox.h"

namespace pt2
{

class OBB : public BoundingBox
{
public:
	OBB();

	virtual BoundingBox* Clone() const override;
	virtual std::unique_ptr<BoundingBox> Clone2() const override;

	virtual size_t Size() const override { return sizeof(OBB); }

	virtual bool IsContain(const sm::vec2& pos) const override;
	virtual bool IsContain(const sm::rect& rect) const override;
	virtual bool IsIntersect(const sm::rect& rect) const override;

	virtual void CombineTo(sm::rect& r) const override;

	virtual void Build(const sm::rect& r, const sm::vec2& pos, float angle,
		const sm::vec2& scale, const sm::vec2& shear, const sm::vec2& offset) override;
	virtual void SetTransform(const sm::vec2& position, const sm::vec2& offset, float angle) override;

	virtual void GetBoundPos(CU_VEC<sm::vec2>& bound) const override;

private:
	sm::rect m_rect;
	sm::vec2 m_position;
	float    m_angle;

}; // OBB

}

#include "painting2/OBB.inl"
