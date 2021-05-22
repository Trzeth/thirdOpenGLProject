#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <vector>

#include <optional>

class Transform
{
public:
	Transform();
	Transform(glm::vec3 position);
	Transform(glm::vec3 position, glm::quat rotation);
	Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	glm::vec3 GetPosition() const;
	glm::vec3 GetWorldPosition() const;
	void SetPosition(glm::vec3 newPosition);

	glm::quat GetRotation() const;
	glm::quat GetWorldRotation() const;
	void SetRotation(glm::quat newRotation);

	glm::vec3 GetScale() const;
	glm::vec3 GetWorldScale() const;
	void SetScale(glm::vec3 newScale);

	glm::vec3 GetForward() const;
	glm::vec3 GetWorldForward() const;

	glm::mat4 Matrix() const;
	glm::mat4 Matrix();

	void SetParent(const std::shared_ptr<Transform>& parent);

	static const Transform identity;

	static const glm::vec3 UP;
	static const glm::vec3 RIGHT;
	static const glm::vec3 FORWARD;
private:
	glm::mat4 getParentMat4() const;
	const std::shared_ptr<Transform> getParentInternal() const;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::mat4 cacheMatrix;
	bool dirty;

	std::optional<std::weak_ptr<Transform>> parent;
	glm::mat4 toMat4() const;
	bool hasParent() const;

	static const glm::mat4 identityMat4;
	static const std::shared_ptr<Transform> identityPtr;
};