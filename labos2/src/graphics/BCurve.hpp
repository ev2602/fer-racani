#ifndef BCURVE_H
#define BCURVE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"

class BCurve {
public:
	std::vector<glm::vec3> controlVertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> curveVertices;

	unsigned int VAO[3];

	Shader shader;

	BCurve();

	void loadCurve(std::string const fname, int divided);

	glm::vec3 getSegment(int i, float t);

	glm::vec3 getTangent(int i, float t);

	glm::vec3 getNormal(int i, float t);

	glm::vec3 getBinormal(glm::vec3 tangent, glm::vec3 normal);

	void drawCurve();

	void drawTangent(int i, float t);


private:
	unsigned int VBO[3];

	void calculateCurvePoints(int divs); 

	void setupCurve();


};

#endif
