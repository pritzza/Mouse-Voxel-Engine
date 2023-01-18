#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(
		const std::string_view& name,
		int startingWidth,
		int startinHeight,
		int majorVerson,
		int minorVersion,
		int profile
	);

public:
	void swapBuffers();

	void pollEvents();

	bool shouldClose() const;

	// getters
	GLFWwindow* const getWindowPtr() const { return window; }
	int getWidth() const	{ return width;  }
	int getHeight() const	{ return height; }

private:
	GLFWwindow* window;
	int width;
	int height;

private:
	void setSize(unsigned w, unsigned h);

	void setCallbacks();

	// call backs
	static void frameBufferResizeCallback(
		GLFWwindow* window, 
		int width, 
		int height
	);

	/*
	* other call backs
	glfwSetWindowPosCallback			(GLFWwindow* w, GLFWwindowposfun callback)
	glfwSetWindowCloseCallback			(GLFWwindow* w, GLFWwindowclosefun callback)
	glfwSetWindowRefreshCallback		(GLFWwindow* w, GLFWwindowrefreshfun callback)
	glfwSetWindowFocusCallback			(GLFWwindow* w, GLFWwindowfocusfun callback)
	glfwSetWindowIconifyCallback		(GLFWwindow* w, GLFWwindowiconifyfun callback)
	glfwSetWindowMaximizeCallback		(GLFWwindow* w, GLFWwindowmaximizefun callback)
	glfwSetFramebufferSizeCallback		(GLFWwindow* w, GLFWframebuffersizefun callback)
	glfwSetWindowContentScaleCallback	(GLFWwindow* w, GLFWwindowcontentscalefun callback)
	*/
};