#pragma once

#include "Frustum.h"

class Camera
{
public:
	Camera();
	Camera(float fieldOfView, unsigned int width, unsigned int height, float nearClip, float farClip);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	glm::mat4 GetViewMatrixOrtho();
	glm::mat4 GetProjectionMatrixOrtho();

	glm::vec2 WorldToScreenPoint(glm::vec3 worldPoint);

	void SetInverseViewMatrix(const glm::mat4& inverseViewMatrix);
	glm::mat4 GetInverseViewMatrix();

	Frustum GetFrustum();
	Frustum GetFrustum() const;
private:
	float fieldOfView;
	unsigned int width;
	unsigned int height;
	float nearClip;
	float farClip;

	glm::mat4 inverseViewMatrix;

	Frustum computeFrustum() const;

	Frustum cachedFrustum;
	bool frustumIsDirty;
};