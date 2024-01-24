#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 modelView = view * model;

	modelView[0][0] = length(vec3(model[0])); 
	modelView[0][1] = 0.0f; 
	modelView[0][2] = 0.0f;

	modelView[1][0] = 0.0f; 
    modelView[1][1] = length(vec3(model[1]));
    modelView[1][2] = 0.0f;

	modelView[2][0] = 0.0f; 
	modelView[2][1] = 0.0f;
	modelView[2][2] = 1.0f; 

    TexCoords = aTexCoords;
	gl_Position = projection * modelView * vec4(aPos, 1.0f);
} 