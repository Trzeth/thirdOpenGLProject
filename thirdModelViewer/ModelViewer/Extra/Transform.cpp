#include "Transform.h"

const glm::vec3 Transform::UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Transform::RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Transform::FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);

const Transform Transform::identity = Transform();
const std::shared_ptr<Transform> Transform::identityPtr(new Transform());
const glm::mat4 Transform::identityMat4 = glm::mat4(1.0f);

Transform::Transform()
	: Transform(glm::vec3(0, 0, 0), glm::quat(), glm::vec3(1, 1, 1)) { }

Transform::Transform(glm::vec3 position)
	: Transform(position, glm::quat(), glm::vec3(1, 1, 1)) { }

Transform::Transform(glm::vec3 position, glm::quat rotation)
	: Transform(position, rotation, glm::vec3(1, 1, 1)) { }

Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: position(position), rotation(rotation), scale(scale), dirty(true) { }

glm::vec3 Transform::GetPosition() const
{
	return this->position;
}

glm::vec3 Transform::GetWorldPosition() const
{
	if (this->hasParent()) {
		const std::shared_ptr<Transform> parent = this->getParentInternal();
		return parent->GetWorldPosition() + parent->GetWorldRotation() * (parent->GetWorldScale() * this->position);
	}
	else
	{
		return this->position;
	}
}

void Transform::SetPosition(glm::vec3 newPosition)
{
	this->position = newPosition;
	this->dirty = true;
}

glm::quat Transform::GetRotation() const
{
	return this->rotation;
}

glm::quat Transform::GetWorldRotation() const
{
	if (this->hasParent()) {
		return this->getParentInternal()->GetWorldRotation() * this->rotation;
	}
	else {
		return this->rotation;
	}
}

void Transform::SetRotation(glm::quat newRotation)
{
	this->rotation = newRotation;
	this->dirty = true;
}

glm::vec3 Transform::GetScale() const
{
	return this->scale;
}

glm::vec3 Transform::GetWorldScale() const
{
	if (this->hasParent()) {
		return this->getParentInternal()->GetWorldScale() * this->scale;
	}
	else {
		return this->scale;
	}
}

void Transform::SetScale(glm::vec3 newScale)
{
	this->scale = newScale;
	this->dirty = true;
}

glm::vec3 Transform::GetForward() const
{
	return rotation * FORWARD;
}

glm::vec3 Transform::GetWorldForward() const
{
	return GetWorldRotation() * FORWARD;
}

glm::mat4 Transform::Matrix() const
{
	if (!this->dirty) {
		return this->getParentMat4() * this->cacheMatrix;
	}
	else {
		return this->getParentMat4() * this->toMat4();
	}
}

glm::mat4 Transform::Matrix()
{
	if (this->dirty) {
		this->cacheMatrix = toMat4();
		this->dirty = false;
	}

	return this->getParentMat4() * this->cacheMatrix;
}

void Transform::SetParent(const std::shared_ptr<Transform>& parent)
{
	this->parent.emplace(std::weak_ptr<Transform>(parent));
}

glm::mat4 Transform::getParentMat4() const
{
	if (this->hasParent()) {
		return this->parent.value().lock()->Matrix();
	}
	else
	{
		return identityMat4;
	}
}

const std::shared_ptr<Transform> Transform::getParentInternal() const
{
	if (this->hasParent()) {
		return this->parent.value().lock();
	}
	else
	{
		return identityPtr;
	}
}

glm::mat4 Transform::toMat4() const
{
	glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotMatrix(glm::toMat4(rotation));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), scale);

	return posMatrix * rotMatrix * scaleMatrix;
}

bool Transform::hasParent() const
{
	return this->parent && !this->parent->expired();
}