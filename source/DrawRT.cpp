#include "painting2/DrawRT.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <gimg_typedef.h>
#include <gimg_export.h>

namespace pt2
{

DrawRT::DrawRT()
{
	auto& rc = Blackboard::Instance()->GetRenderContext();
	auto& rt_mgr = rc.GetRTMgr();
	m_rt = rt_mgr.Fetch();
}

uint8_t* DrawRT::StoreToMemory(int width, int height, int channels)
{
	if (channels != 3 && channels != 4) {
		return nullptr;
	}

	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	int sz = width * height * channels;
	uint8_t* pixels = new uint8_t[sz];
	if (!pixels) {
		return nullptr;
	}

	memset(pixels, 0, sz);

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	m_rt->Bind();
	ur_rc.ReadPixels(pixels, channels, 0, 0, width, height);
	m_rt->Unbind();
	return pixels;
}

void DrawRT::StoreToFile(const std::string& filepath, int width, int height, int channels)
{
	if (channels != 3 && channels != 4) {
		return;
	}

	uint8_t* pixels = StoreToMemory(width, height, channels);
	if (!pixels) {
		return;
	}

	GIMG_PIXEL_FORMAT fmt = channels == 3 ? GPF_RGB : GPF_RGBA8;
	gimg_export(filepath.c_str(), pixels, width, height, fmt, false);

	delete[] pixels;
}

}