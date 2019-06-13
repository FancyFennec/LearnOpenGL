#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"

class Window
{
public:
	Window();
	Window(unsigned int width,unsigned int height);
	~Window();

	int initialise();
	void processInput(float deltaTime);

	bool getShouldClose() { return glfwWindowShouldClose(window); }
	void swapBuffers() { glfwSwapBuffers(window); }
	GLFWwindow* getWindow() { return window;  }

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow* window;

	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;

	static bool firstMouse;

	static float lastX;
	static float lastY;

	static Camera camera;
};

