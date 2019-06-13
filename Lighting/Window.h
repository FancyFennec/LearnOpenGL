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

	//void setCamera(Camera camera) { this->camera = camera;  }
	Camera getCamera() { return camera; }
	bool getShouldClose() { return glfwWindowShouldClose(window); }
	GLFWwindow* getWindow() { return window;  }

private:
	GLFWwindow* window;

	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;

	static bool firstMouse;
	static bool cursorEnabled;

	static float lastX;
	static float lastY;

	static Camera camera;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

