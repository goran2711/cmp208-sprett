#include "Camera.h"
#include "GameObject.h"

#include <system/platform.h>

Camera::Camera(const gef::Vector4& position, GameObject* target, float fovDeg, float near, float far)
	:	m_position(position)
	,	m_target(target)
	,	m_fov(gef::DegToRad(fovDeg))
	,	m_near(near)
	,	m_far(far)
{
	m_lookAt = m_position;

	if (m_target)
		m_lookAt.set_z(m_target->getPositionVec4().z());
	else
		m_lookAt.set_z(0.f);
}

void Camera::update()
{
	if (!m_target)
		return;

	// Only update the lookAt y-coordinate if the target's (player's) is higher
	auto targetY = m_target->getPositionVec2().y;
	if (targetY < m_position.y())
		return;

	m_position.set_y(targetY);
	m_lookAt.set_y(targetY);
}

void Camera::reset(const gef::Vector4& position)
{
	m_position = position;

	if (m_target)
		m_lookAt = { m_position.x(), m_position.y(), m_target->getPositionVec4().z() };
	else
		m_lookAt = { m_position.x(), m_position.y(), 0.f };
}

gef::Matrix44 Camera::getViewMatrix() const
{
	gef::Matrix44 matrix;
	matrix.LookAt(m_position, m_lookAt, { 0.f, 1.f, 0.f });
	return matrix;
}

gef::Matrix44 Camera::getProjectionMatrix(gef::Platform& platform) const
{
	return platform.PerspectiveProjectionFov(m_fov, ASPECT_RATIO, m_near, m_far);
}
