#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "graphics/Shader.hpp"
#include "graphics/Model.hpp"

#include "Camera.hpp"
#include "Screen.hpp"
#include "graphics/BCurve.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);

float mixVal = 0.5f;

Camera camera(glm::vec3(0.0f, 0.0f, 60.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

Screen screen;

std::vector<glm::vec4> controlPoints;


int main() {
	std::cout << "Hello world!" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!screen.init()) {
		std::cout << "Could not create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize glad" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	glEnable(GL_DEPTH_TEST);
	

	//Shaders

	Shader shader("shaders/vertex_core.glsl", "shaders/fragment_core.glsl");
	Shader shader_curve("shaders/curve_vertex.glsl", "shaders/curve_fragment.glsl");

	Model model("src/resource/bird.obj");
	BCurve bSplineCurve;
	bSplineCurve.loadCurve("src/resource/BSplineCurve.obj", 100);
	bSplineCurve.shader = shader_curve;

	int segment = 1;
	float t = 0.0;
	float speed = 0.0001f;

	//render loop that keeps on running until we tell GLFW to stop
	while (!screen.shouldClose()) {

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//process input
		processInput(screen.getWindow(), deltaTime);

		//render
		screen.update();
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		t += speed;
		if (t >= 1.0f) {
			t = 0.0f;
			segment++;
			if (segment > bSplineCurve.controlVertices.size() - 3) {
				segment = 1;
			}
		}
	

		glm::vec3 newPosition = bSplineCurve.getSegment(segment, t);
		glm::vec3 tangent = bSplineCurve.getTangent(segment, t);
		glm::vec3 normal = bSplineCurve.getNormal(segment, t);
		glm::vec3 binormal = bSplineCurve.getBinormal(tangent, normal);

		model.updatePosition(newPosition);
		model.updateOrientation(tangent, normal, binormal);
	

		shader.activate();
		shader.set3Float("viewPos", camera.cameraPos);
		shader.setFloat("mixVal", mixVal);

		model.Draw(shader);

		// create transformation
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 projection_curve = glm::mat4(1.0f);
		glm::mat4 model2 = glm::mat4(1.0f);

		view = camera.getViewMatrix();
		projection = camera.getPerspectiveMatrix();
		model2 = model.getModelMatrix();

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setMat4("model", model2);



		//curve render
		shader_curve.activate();
		//set color
		GLint colorLocation = glGetUniformLocation(shader_curve.id, "lineColor");
		glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);  //red

		glEnable(GL_PROGRAM_POINT_SIZE);

		bSplineCurve.drawCurve();

		glUniform3f(colorLocation, 1.0f, 1.0f, 0.0f); //yellow

		bSplineCurve.drawTangent(segment, t);

		//transformation
		model2 = glm::mat4(1.0f);
		projection_curve = camera.getPerspectiveMatrix();
		shader_curve.setMat4("view", view);
		shader_curve.setMat4("projection", projection_curve);

		//send new frame to window
		screen.newFrame();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void processInput(GLFWwindow* window, double dt)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		screen.setShouldClose(true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::UP, dt);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.updateCameraPos(CameraDirection::DOWN, dt);
	}
}
  