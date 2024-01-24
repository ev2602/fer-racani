#include "Transform.hpp"

Transform::Transform() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	orientation = glm::mat4(1.0f);
	viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
	size = 1.0f;

	translationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::mat4(1.0f);
}

//updates translation matrix 
void Transform::updatePosition(glm::vec3 position) {
	translationMatrix = Transform::translate3D(position);
}

//updates rotation matrix
void Transform::updateOrientation(glm::vec3 x, glm::vec3 y, glm::vec3 z) {
	rotationMatrix = coordinatesRotation3D(x, y, z);
}


//transforms in direction translateVector
glm::mat4 Transform::translate3D(glm::vec3 translateVector) {
	glm::mat4 trans = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translateVector.x, translateVector.y, translateVector.z, 1.0f);
	return trans;
}

//rotates around given axis and angle
glm::mat4 Transform::rotate3D(float angle, glm::vec3 axis) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(angle), axis);
	return trans;
}

//rotates so its local coordinates match given ones
glm::mat4 Transform::coordinatesRotation3D(glm::vec3 w, glm::vec3 u, glm::vec3 v) {
	glm::mat4 trans = glm::mat4(
		w.x, w.y, w.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		v.x, v.y, v.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return trans;
}

//scaling matrix 
glm::mat4 Transform::scale3D(glm::vec3 scaleVector) {
	glm::mat4 trans = glm::mat4(
		scaleVector.x, 0.0f, 0.0f, 0.0f,
		0.0f, scaleVector.y, 0.0f, 0.0f,
		0.0f, 0.0f, scaleVector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return trans;
}

//projection matrix for a perspective frustum
glm::mat4 Transform::frustum(float l, float r, float b, float t, float n, float f) {
	glm::mat4 projection = glm::mat4(
		(2 * n) / (r - l), 0.0f, 0.0f, 0.0f,
		0.0f, (2 * n) / (t - b), 0.0f, 0.0f,
		(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f,
		0.0f, 0.0f, -(2 * f * n) / (f - n), 0.0f);

	return projection;
}

void Transform::addTransformation(glm::mat4 trans) {
	transformations.push_back(trans);
}

void Transform::translate(glm::vec3 direction) {
	glm::mat4 trans = translate3D(direction);
	addTransformation(trans);
}

void Transform::rotate(float angle, glm::vec3 axis) {
	glm::mat4 trans = rotate3D(angle, axis);
	addTransformation(trans);
}

void Transform::scale(glm::vec3 factors) {
	glm::mat4 trans = scale3D(factors);
	addTransformation(trans);
}

//creates model matrix
glm::mat4 Transform::getModelMatrix() {
	glm::mat4 model = glm::mat4(1.0f);

	model = translationMatrix * glm::inverse(glm::transpose(rotationMatrix)) * orientation * model;

	return model;
}