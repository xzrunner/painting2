#include "painting2/Pseudo3DCamera.h"
#include "painting2/WindowContext.h"
#include "painting2/Blackboard.h"
#include "painting2/RenderContext.h"

#ifndef PT2_DISABLE_CAMERA25

#include <c25_camera.h>
#include <sm_c_vector.h>

#include <stddef.h>

namespace pt2
{

//static const float Z = -1000;
static const float Z = -250;
static const float ANGLE = -20;

Pseudo3DCamera::Pseudo3DCamera()
	: m_cam(nullptr)
{
	//auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	//if (wc)
	//{
	//	float w = static_cast<float>(wc->GetScreenWidth());
	//	float h = static_cast<float>(wc->GetScreenHeight());
	//	OnSize(w, h);
	//}
}

Pseudo3DCamera::Pseudo3DCamera(const Pseudo3DCamera& cam)
{
	Init(cam);
}

Pseudo3DCamera& Pseudo3DCamera::operator = (const Pseudo3DCamera& cam)
{
	Init(cam);
	return *this;
}

Pseudo3DCamera::~Pseudo3DCamera()
{
	c25_cam_release(m_cam);
}

void Pseudo3DCamera::OnSize(float width, float height)
{
    pt0::Camera::OnSize(width, height);

	//auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	//if (wc) {
	//	wc->SetProjection(static_cast<int>(width), static_cast<int>(height));
	//}

	c25_cam_release(m_cam);

	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	m_cam = c25_cam_create(&pos, ANGLE, (float)width / height);

	UpdateRender();
}

void Pseudo3DCamera::Bind() const
{
	UpdateRender();
}

sm::mat4 Pseudo3DCamera::GetViewMat() const
{
	auto ptr = c25_cam_get_modelview_mat(m_cam);
	sm::mat4 ret;
	memcpy(ret.x, ptr, sizeof(ret.x));
	return ret;
}

sm::mat4 Pseudo3DCamera::GetProjectionMat() const
{
	auto ptr = c25_cam_get_project_mat(m_cam);
	sm::mat4 ret;
	memcpy(ret.x, ptr, sizeof(ret.x));
	return ret;
}

void Pseudo3DCamera::Reset()
{
	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	c25_cam_set_pos(m_cam, &pos);

	c25_cam_set_angle(m_cam, ANGLE);

	UpdateRender();
}

sm::vec2 Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm_ivec2 screen;
	screen.x = x;
	screen.y = y;

	sm_vec2 world;
	c25_screen_to_world(m_cam, &world, &screen, width, height);

	return sm::vec2(world.x, world.y);
}

sm::vec2 Pseudo3DCamera::TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const
{
	sm_vec3 world;
	world.x = proj.x;
	world.y = proj.y;
	world.z = proj.z;

	sm_ivec2 screen;
	c25_world_to_screen(m_cam, &screen, &world, width, height);

	return sm::vec2((float)screen.x, (float)screen.y);
}

void Pseudo3DCamera::Translate(const sm::vec3& offset)
{
	sm_vec3 vec;
	vec.x = offset.x;
	vec.y = offset.y;
	vec.z = offset.z;
	c25_cam_translate(m_cam, &vec);

	UpdateRender();
}

void Pseudo3DCamera::Rotate(float da)
{
	c25_cam_rotate(m_cam, da);

	UpdateRender();
}

float Pseudo3DCamera::GetAngle() const
{
	return c25_cam_get_angle(m_cam);
}

const sm_vec3* Pseudo3DCamera::GetPos() const
{
	return c25_cam_get_pos(m_cam);
}

void Pseudo3DCamera::Init(const Pseudo3DCamera& cam)
{
	//auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	//if (!wc) {
	//	return;
	//}

	const sm_vec3* pos = c25_cam_get_pos(cam.m_cam);
	float angle = c25_cam_get_angle(cam.m_cam);
    auto& sz = GetSize();
	m_cam = c25_cam_create(pos, angle, sz.x / sz.y);
}

void Pseudo3DCamera::UpdateRender() const
{
	float angle = c25_cam_get_angle(m_cam);

	// todo: should change RenderContext, which will change sl mvp
// 	const sm_mat4* mat = c25_cam_get_modelview_mat(m_cam);
// 	sl::SubjectMVP3::Instance()->NotifyModelview(*(const sm::mat4*)mat);
}

}

#endif // PT2_DISABLE_CAMERA25