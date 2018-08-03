#pragma once

#include "painting2/config.h"

#ifndef PT2_DISABLE_CAMERA25

#include <painting0/Camera.h>

#include <SM_Vector.h>
#include <SM_Matrix.h>

struct c25_camera;
struct sm_vec3;
union sm_mat4;

namespace pt2
{

class Pseudo3DCamera : public pt0::Camera
{
public:
	Pseudo3DCamera();
	Pseudo3DCamera(const Pseudo3DCamera& cam);
	Pseudo3DCamera& operator = (const Pseudo3DCamera& cam);
	virtual ~Pseudo3DCamera();

	virtual pt0::CamTypeID TypeID() const override {
		return pt0::GetCamTypeID<Pseudo3DCamera>();
	}

	virtual void OnSize(float width, float height) override;

	virtual void Bind() const override;

	virtual sm::mat4 GetModelViewMat() const override;
	virtual sm::mat4 GetProjectionMat() const override;

	virtual void Reset() override;

	sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	sm::vec2 TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const;

	void Translate(const sm::vec3& offset);
	void Rotate(float da);

	float GetAngle() const;
	const sm_vec3* GetPos() const;

private:
	void Init(const Pseudo3DCamera& cam);

	void UpdateRender() const;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // PT2_DISABLE_CAMERA25