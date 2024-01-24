#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Shader.hpp"


struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
	
};

typedef struct Vertex Vertex;

struct Texture {
	unsigned int id;
	std::string type;
    std::string path;
};

typedef struct Texture Texture;


class Mesh {
public:
    //mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
    void updateMesh();
    std::vector<Vertex> getBoundingBox();
    void applyTransform(glm::mat4 transformationMatrix);

private:
    //render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif