//#include "painting2/Texture.h"
//
//namespace pt2
//{
//
//Texture::Texture(ur::RenderContext* rc, uint16_t w, uint16_t h, uint32_t id, int format)
//	: m_ori_w(0)
//	, m_ori_h(0)
//	, m_load_finished(true)
//{
//	Init(rc, w, h, id, format);
//}
//
//void Texture::Init(ur::RenderContext* rc, uint16_t w, uint16_t h, uint32_t id, int format)
//{
//	m_rc     = rc;
//	m_width  = w;
//	m_height = h;
//	m_texid  = id;
//	m_format = static_cast<ur::TEXTURE_FORMAT>(format);
//}
//
//void Texture::InitOri(uint16_t ori_w, uint16_t ori_h)
//{
//	m_ori_w = ori_w;
//	m_ori_h = ori_h;
//}
//
//}