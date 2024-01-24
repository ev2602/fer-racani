#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "./Model.hpp"


//represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{
public:

    ParticleGenerator(Shader shader, const char* texturePath, unsigned int amount);

    void Update(float dt, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
    void Draw(glm::mat4 view, glm::mat4 projection);

private:
    // img properties
    const char* texturePath;
    int width;
    int height;
    int nChannels;

    std::vector<Particle> particles;
    unsigned int amount;

    Shader shader;
    unsigned int textureID;

    unsigned int VAO; 

    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

    void loadTexture(bool flip);

    glm::mat4 getParticleModelMatrix(Particle p);

};

#endif