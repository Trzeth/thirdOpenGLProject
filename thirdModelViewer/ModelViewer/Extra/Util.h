#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <string>
using namespace std;

static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec3 vec3_cast(const btVector3& v) { return glm::vec3(v.x(), v.y(), v.z()); }
static inline btVector3 vec3_cast(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }

static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); } // it's aiVector3D because assimp's texture coordinates use that

static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::quat quat_cast(const btQuaternion& q) { return glm::quat(q.w(), q.x(), q.y(), q.z()); }
static inline btQuaternion quat_cast(const glm::quat& q) { return btQuaternion(q.w, q.x, q.y, q.z); }

static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static inline glm::mat4 mat4_cast(const aiMatrix3x3& m) { return glm::transpose(glm::make_mat3(&m.a1)); }
static inline btTransform mat4_cast(const glm::mat4& m)
{
	btTransform t;
	t.setFromOpenGLMatrix(glm::value_ptr(m));
	return t;
}

GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)
