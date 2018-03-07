#pragma once

#include "painting2/SRT.h"

#include <SM_Matrix2D.h>

namespace pt2
{

class GeoTransform
{
public:
	GeoTransform();

	void Reset();

	const sm::vec2& GetPosition() const { return m_srt.position; }
	float GetAngle() const              { return m_srt.angle; }
	const sm::vec2& GetScale() const    { return m_srt.scale; }
	const sm::vec2& GetShear() const    { return m_srt.shear; }
	const sm::vec2& GetOffset() const   { return m_srt.offset; }
	const sm::vec2& GetCenter() const   { return m_srt.center; }

	SRT& GetSRT()                         { return m_srt; }
	const SRT& GetSRT() const             { return m_srt; }
	const sm::Matrix2D& GetMatrix() const { return m_mat; }

	bool SetPosition(const sm::vec2& pos);
	bool SetAngle(float angle);
	bool SetScale(const sm::vec2& scale);
	bool SetShear(const sm::vec2& shear);
	bool SetOffset(const sm::vec2& offset);

	bool SetSRT(const SRT& srt);

private:
	void UpdateSrt();
	void UpdateMatrix();

private:
	SRT          m_srt;
	sm::Matrix2D m_mat;

}; // GeoTransform

}

#include "painting2/GeoTransform.inl"