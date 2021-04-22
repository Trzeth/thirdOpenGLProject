#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include <glm/gtx/projection.hpp>

Camera::Camera()
	: fieldOfView(glm::radians(45.0f)),
	width(0),
	height(0),
	nearClip(0.1f),
	farClip(10000),
	inverseViewMatrix(1.0f),
	frustumIsDirty(true)
{ }

Camera::Camera(float fieldOfView, unsigned int width, unsigned int height, float nearClip, float farClip)
	: fieldOfView(fieldOfView),
	width(width),
	height(height),
	nearClip(nearClip),
	farClip(farClip),
	inverseViewMatrix(1.0f),
	frustumIsDirty(true)
{ }

glm::mat4 Camera::GetViewMatrix()
{
	return glm::inverse(inverseViewMatrix);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if (height == 0)
		return glm::mat4(1.0f);

	return glm::perspective(fieldOfView, (float)width / (float)height, nearClip, farClip);
}

glm::mat4 Camera::GetViewMatrixOrtho()
{
	return glm::mat4(1.0f);
}

glm::mat4 Camera::GetProjectionMatrixOrtho()
{
	// Reverse y direction since that's the way 2D applications normally have it
	return glm::ortho(0.0f, (float)width, (float)height, 0.0f);
}

glm::vec2 Camera::WorldToScreenPoint(glm::vec3 worldPoint)
{
	glm::vec4 clipSpacePos = this->GetProjectionMatrix() * (this->GetViewMatrix() * glm::vec4(worldPoint, 1.0f));
	glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;
	ndcSpacePos.y = -ndcSpacePos.y;
	glm::vec2 windowSpacePos = (glm::vec2(ndcSpacePos) + glm::vec2(1.0f)) / 2.0f * glm::vec2(width, height);
	return windowSpacePos;
}

void Camera::SetInverseViewMatrix(const glm::mat4& inverseViewMatrix)
{
	this->inverseViewMatrix = inverseViewMatrix;
	frustumIsDirty = true;
}

glm::mat4 Camera::GetInverseViewMatrix()
{
	return this->inverseViewMatrix;
}

Frustum Camera::GetFrustum() const
{
	if (frustumIsDirty) {
		return this->computeFrustum();
	}
	else {
		return this->cachedFrustum;
	}
}

void Camera::SetFOV(float fov)
{
	fieldOfView = fov;
}

void Camera::SetScreenSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
}

void Camera::SetClip(float near, float far)
{
	nearClip = near;
	farClip = far;
}

Frustum Camera::GetFrustum()
{
	if (frustumIsDirty) {
		this->cachedFrustum = this->computeFrustum();
	}
	return this->cachedFrustum;
}

Frustum Camera::computeFrustum() const
{
	float nearHeight = 2.0f * tan(fieldOfView / 2.0f) * nearClip;
	float nearWidth = nearHeight * width / height;
	float farHeight = 2.0f * tan(fieldOfView / 2.0f) * farClip;
	float farWidth = farHeight * width / height;

	// Fwd is negated because negative z == forward
	glm::vec3 cameraPos = glm::vec3(inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
	glm::vec3 cameraFwd = -glm::vec3(inverseViewMatrix[2][0], inverseViewMatrix[2][1], inverseViewMatrix[2][2]);
	glm::vec3 cameraUp = glm::vec3(inverseViewMatrix[1][0], inverseViewMatrix[1][1], inverseViewMatrix[1][2]);
	glm::vec3 cameraRight = glm::vec3(inverseViewMatrix[0][0], inverseViewMatrix[0][1], inverseViewMatrix[0][2]);

	glm::vec3 nearCenter = cameraPos + cameraFwd * nearClip;
	glm::vec3 nearX = cameraUp * nearHeight / 2.0f;
	glm::vec3 nearY = cameraRight * nearWidth / 2.0f;
	glm::vec3 farCenter = cameraPos + cameraFwd * farClip;
	glm::vec3 farX = cameraUp * farHeight / 2.0f;
	glm::vec3 farY = cameraRight * farWidth / 2.0f;

	FrustumConstructor f;
	f.ntr = nearCenter + nearX + nearY;
	f.ntl = nearCenter + nearX - nearY;
	f.nbr = nearCenter - nearX + nearY;
	f.nbl = nearCenter - nearX - nearY;
	f.ftr = farCenter + farX + farY;
	f.ftl = farCenter + farX - farY;
	f.fbr = farCenter - farX + farY;
	f.fbl = farCenter - farX - farY;

	return Frustum(f);
}