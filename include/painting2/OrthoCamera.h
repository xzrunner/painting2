#pragma once

#include "painting2/Camera.h"
#include "painting2/CameraType.h"

#include <SM_Vector.h>

namespace pt2
{

class OrthoCamera : public Camera
{
public:
	OrthoCamera();
	
	virtual CameraType Type() const override { return CAM_ORTHO2D; }
	virtual void OnSize(int width, int height) override;
	virtual void Reset() override;
	virtual void Bind() const override;

	sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const;

	void Translate(const sm::vec2& offset);
	void Scale(float scale, int x, int y, int width, int height);

	float GetScale() const { return m_scale; }
	const sm::vec2& GetPosition() const { return m_position; }
	void Set(const sm::vec2& pos, float scale);

private:
	void UpdateRender() const;

private:
	sm::vec2 m_position;
	float    m_scale;

}; // OrthoCamera

}