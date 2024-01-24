#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    //vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
	//unnecessary for this task
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); //activate proper texture unit before binding
        //retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    //draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::updateMesh() {
	//update vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

std::vector<Vertex> Mesh::getBoundingBox() {

	float xmin, xmax, ymin, ymax, zmin, zmax;
	Vertex vmin, vmax;
	std::vector<Vertex> boundingBox;

	xmin = xmax = vertices[0].Position.x;
	ymin = ymax = vertices[0].Position.y;
	zmin = zmax = vertices[0].Position.z;

	for (int i = 1; i < vertices.size(); i++) {
		xmin = std::min(vertices[i].Position.x, xmin);
		ymin = std::min(vertices[i].Position.y, ymin);
		zmin = std::min(vertices[i].Position.z, zmin);
		xmax = std::max(vertices[i].Position.x, xmax);
		ymax = std::max(vertices[i].Position.y, ymax);
		zmax = std::max(vertices[i].Position.z, zmax);
	}

	vmin.Position.x = xmin;
	vmin.Position.y = ymin;
	vmin.Position.z = zmin;
	vmax.Position.x = xmax;
	vmax.Position.y = ymax;
	vmax.Position.z = zmax;

	boundingBox.push_back(vmin);
	boundingBox.push_back(vmax);

	return boundingBox;
}

void Mesh::applyTransform(glm::mat4 transformationMatrix) {
	for (int i = 0; i < vertices.size(); i++) {

		glm::vec4 vertex = glm::vec4(
			vertices[i].Position.x,
			vertices[i].Position.y,
			vertices[i].Position.z,
			1.0f);
		//calculates the transformation
		vertex = transformationMatrix * vertex;

		float h = vertex[3];
		//remove homogeneous coordinate
		glm::vec3 newVertex = (1 / h) * glm::vec3(vertex[0], vertex[1], vertex[2]);

		//new vertex
		vertices[i].Position.x = newVertex.x;
		vertices[i].Position.y = newVertex.y;
		vertices[i].Position.z = newVertex.z;
	}
	updateMesh();
}
