#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"

class Window
{
public:
	Window();
	Window(Camera camera);
	Window(unsigned int width,unsigned int height);
	Window(Camera camera, unsigned int width, unsigned int height);
	~Window();

	int initialise();
	void processInput(float deltaTime);
	void swapBuffers() { glfwSwapBuffers(window); }

	void setCamera(Camera camera) { this->camera = camera;  }
	bool getShouldClose() { return glfwWindowShouldClose(window); }
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

