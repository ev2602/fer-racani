#ifndef SCRREN_H
#define SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Screen {
public:
	static unsigned int SCREEN_WIDTH;
	static unsigned int SCREEN_HEIGHT;

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	Screen();

	bool init();

	void setParameters();

	//main loop
	void update();
	void newFrame();

	//window closing accessor and modifier
	bool shouldClose();
	void setShouldClose(bool shouldClose);

	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};

#endif