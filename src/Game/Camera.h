#pragma once
#include "../Common/Common.h"

#include <gef.h>
#include <cmath>
#include <maths/vector4.h>
#include <maths/math_utils.h>

namespace gef
{
	class Platform;
}

class GameObject;

class Camera
{
public:
	Camera() = default;
	Camera(const gef::Vector4& position, GameObject* target, float fovDeg, float near = 0.1f, float far = 100.f);

	void update();
	void reset(const gef::Vector4& position);

	void setTarget(GameObject* target) { m_target = target; }
	void setPosition(const gef::Vector4& position) { m_position = position; }

	gef::Matrix44 getViewMatrix() const;
	gef::Matrix44 getProjectionMatrix(gef::Platform& platform) const;

	gef::Vector4 getPosition() const { return m_position; }

	// Accessors to get the dimensions of the xy-plane at z coordinate 0
	float getViewPlaneWidth() const { return (std::tanf(m_fov * 0.5f) * m_position.z() * 2.f) * ASPECT_RATIO; }
	float getViewPlaneHeight() const { return std::tanf(m_fov * 0.5f) * m_position.z() * 2.f; }

	// Accessors to get the coordinates of the different edges of the screen
	float getViewPlaneTop() const { return m_position.y() + (getViewPlaneHeight() * 0.5f); }
	float getViewPlaneBottom() const { return m_position.y() - (getViewPlaneHeight() * 0.5f); }
	float getViewPlaneRight() const { return m_position.x() + (getViewPlaneWidth() * 0.5f); }
	float getViewPlaneLeft() const { return m_position.x() - (getViewPlaneWidth() * 0.5f); }

private:
	gef::Vector4 m_position = { 0.f, 0.f, 0.f };
	gef::Vector4 m_lookAt	= { 0.f, 0.f, 0.f };
	GameObject* m_target	= nullptr;

	float m_fov = gef::DegToRad(70.f);
	float m_near = 0.1f;
	float m_far = 100.f;
};

