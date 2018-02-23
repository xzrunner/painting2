#pragma once

#include "painting2/CameraType.h"

namespace pt2
{

class Camera
{
public:
	virtual ~Camera() {}
	virtual CameraType Type() const = 0;
	virtual void OnSize(int width, int height) = 0;
	virtual void Reset() = 0;
	virtual void Bind() const = 0;

}; // Camera

}