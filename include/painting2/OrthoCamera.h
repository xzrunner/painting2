#pragma once

#include <painting0/Camera.h>

#include <SM_Vector.h>

namespace pt2
{

class OrthoCamera : public pt0::Camera
{
public:
	OrthoCamera();
	
	virtual pt0::CamTypeID TypeID() const override {
		return pt0::GetCamTypeID<OrthoCamera>();
	}

	virtual void OnSize(float width, float height) override;

	virtual void Bind() const override;

	virtual sm::mat4 GetModelViewMat() const override { return sm::mat4(); }
	virtual sm::mat4 GetProjectionMat() const override { return sm::mat4(); }

	virtual void Reset() override;

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