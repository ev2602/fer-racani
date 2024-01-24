#include "Particle.hpp"
#include <glm/gtc/type_ptr.hpp>

ParticleGenerator::ParticleGenerator(Shader shader, const char* texturePath, unsigned int amount)
    : shader(shader), texturePath(texturePath), amount(amount)
{
    loadTexture(false);
    this->init();
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt * 0.4f; // reduce life
        if (p.Life > 0.0f)
        {	//particle is alive, thus update
            p.Position += p.Velocity * dt;
            p.Velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dt * 1.5f;
            p.Color.a -= dt * 1.5f;
            
        }
    }
}

//render all particles
void ParticleGenerator::Draw(glm::mat4 view, glm::mat4 projection)
{

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.activate();
    for (Particle particle : this->particles)
    {
        
        if (particle.Life > 0.0f)
        {
          
            //particle model matrix setup
            unsigned int modelLoc = glGetUniformLocation(shader.id, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(getParticleModelMatrix(particle)));

            //view setup
            unsigned int viewLoc = glGetUniformLocation(shader.id, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            //projection setup
            unsigned int projLoc = glGetUniformLocation(shader.id, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            //particle position
            unsigned int billboardPositionLoc = glGetUniformLocation(shader.id, "billboard_position");
            glUniform4f(billboardPositionLoc, particle.Position.x, particle.Position.y, particle.Position.z, 0.0f);

            //alpha setup
            unsigned int alphaLoc = glGetUniformLocation(shader.id, "alpha");
            glUniform1f(alphaLoc, particle.Life);

            // bind Texture
            //glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(this->VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

        }
        
        
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
         0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

 
    //texture 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

//stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    //first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    //otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    //all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec3 offset)
{
    float random_x = ((rand() % 100) - 50) / 10.0f;
    float random_z = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    float random_v = ((rand() % 100) - 50) / 100.0f;

    //set initial position to origin=offset
    particle.Position = offset;

    //add some randomness to x and z coordinates -> just x
    particle.Position.x += random_x;
    //particle.Position.z += random_z;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity.y = 15.0f + random_v;
    particle.Velocity.x = 2.0f + random_v;
}


void ParticleGenerator::loadTexture(bool flip) {

    stbi_set_flip_vertically_on_load(flip);

    unsigned char* data = stbi_load(texturePath, &width, &height, &nChannels, STBI_rgb);

    GLenum colorMode = GL_RGB;
    switch (nChannels) {
    case 1:
        colorMode = GL_RED;
        break;
    case 4:
        colorMode = GL_RGBA;
        break;
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data) {
        //glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Image not loaded at " << texturePath << std::endl;
    }

    stbi_image_free(data);

}

glm::mat4 ParticleGenerator::getParticleModelMatrix(Particle p) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translateMatrix = Transform::translate3D(p.Position);
    float scale = 1.0f + (1.0f - p.Life) * 2.0f;
    glm::mat4 scaleMatrix = Transform::scale3D(glm::vec3(scale, scale, scale));

    return translateMatrix * scaleMatrix * model;
}
