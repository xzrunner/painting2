#pragma once

#include <unirender/Texture.h>

#include <SM_Vector.h>

#include <stdint.h>

namespace pt2
{

class Texture : public ur::Texture
{
public:
	Texture(ur::RenderContext* rc, uint16_t w, uint16_t h, uint32_t id, int format);

	void Init(ur::RenderContext* rc, uint16_t w, uint16_t h, uint32_t id, int format);
	void InitOri(uint16_t ori_w, uint16_t ori_h);

	sm::vec2 GetSize() const {
		return sm::vec2(static_cast<float>(Width()), static_cast<float>(Height()));
	}
	sm::vec2 GetOriSize() const {
		return sm::vec2(m_ori_w, m_ori_h);
	}

	bool IsLoadFinished() const { return m_load_finished; }
	void SetLoadFinished(bool finished) { m_load_finished = finished; }

private:
	uint16_t m_ori_w, m_ori_h;
//		     m_ori_x, m_ori_y;

	bool m_load_finished;

}; // Texture

}