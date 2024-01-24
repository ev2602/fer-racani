#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Transform {
public:
	glm::vec3 position;
	glm::mat4 orientation;
	glm::vec3 viewUp;
	glm::vec3 velocity;
	float size;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;

	std::vector<glm::mat4> transformations;

	Transform();

	void updatePosition(glm::vec3 position);
	void updateOrientation(glm::vec3 x, glm::vec3 y, glm::vec3 z);

	// functions that generate transformation matrices
	static glm::mat4 translate3D(glm::vec3 translateVector);
	static glm::mat4 rotate3D(float angle, glm::vec3 axis);
	static glm::mat4 coordinatesRotation3D(glm::vec3 w, glm::vec3 u, glm::vec3 v);
	static glm::mat4 scale3D(glm::vec3 scaleVector);
	static glm::mat4 frustum(float l, float r, float b, float t, float n, float f);

	void addTransformation(glm::mat4 trans);

	// functions that accumulate transformations of the original object vertices
	void translate(glm::vec3 translateVector);
	void rotate(float angle, glm::vec3 axis);
	void scale(glm::vec3 scaleVector);

	glm::mat4 getModelMatrix();


};

#endif