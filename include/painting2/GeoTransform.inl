#pragma once

namespace pt2
{

inline
bool GeoTransform::SetPosition(const sm::vec2& pos)
{
	if (pos != m_srt.position)
	{
		m_srt.position = pos;
		UpdateSrt();
		UpdateMatrix();
		return true;
	}
	else
	{
		return false;
	}
}

inline
bool GeoTransform::SetAngle(float angle)
{
	if (angle != m_srt.angle)
	{
		m_srt.angle = angle;
		UpdateSrt();
		UpdateMatrix();
		return true;
	}
	else
	{
		return false;
	}
}

inline
bool GeoTransform::SetScale(const sm::vec2& scale)
{
	if (scale != m_srt.scale)
	{
		m_srt.scale = scale;
		UpdateMatrix();
		return true;
	}
	else
	{
		return false;
	}
}

inline
bool GeoTransform::SetShear(const sm::vec2& shear)
{
	if (shear != m_srt.shear)
	{
		m_srt.shear = shear;
		UpdateMatrix();
		return true;
	}
	else
	{
		return false;
	}
}

inline
bool GeoTransform::SetOffset(const sm::vec2& offset)
{
	if (offset != m_srt.offset)
	{
		m_srt.offset = offset;
		UpdateSrt();
		UpdateMatrix();
		return true;
	}
	else
	{
		return false;
	}
}

inline
bool GeoTransform::SetSRT(const SRT& srt)
{
	m_srt = srt;
	UpdateMatrix();
	return true;
}

inline
void GeoTransform::UpdateSrt()
{
	m_srt.Update();
}

inline
void GeoTransform::UpdateMatrix()
{
	m_mat.SetTransformation(m_srt.center.x, m_srt.center.y, m_srt.angle, m_srt.scale.x,
		m_srt.scale.y, 0, 0, m_srt.shear.x, m_srt.shear.y);
}

}