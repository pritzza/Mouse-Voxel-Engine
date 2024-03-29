#include "Window.h"

#include <cassert>
#include <iostream>

Window::Window(
	const std::string_view& name,
	int startingWidth, 
	int startingHeight,
	int majorVersion,
	int minorVersion,
	int profile
)
	:
	width{ startingWidth },
	height{ startingHeight }
{
	// set hints about window before creating it
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// for MacOS
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(
		startingWidth, 		// width
		startingHeight, 	// height
		name.data(),		// name
		nullptr, 			// monitor
		nullptr				// share
	);

	const bool createdSuccessfully{ window != nullptr };
	if (!createdSuccessfully)
	{
		std::cerr << "Error! Couldn't create GLFW window.\n";
		assert(false);
	}

	// bind opengl context to window
	glfwMakeContextCurrent(window);

	setCallbacks();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::setSize(unsigned w, unsigned h)
{
	width = w;
	height = h;
	glfwSetWindowSize(window, width, height);

	glViewport(0, 0, width, height);
}

void Window::setCallbacks()
{
	// glfw lets you statically bind user defined window class to glfw window
	// instance to use in callbacks
	glfwSetWindowUserPointer(window, (void*)this);

	// set window resize callback
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

	// set other callbacks here
}

void Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	Window& userWindow{ *(Window*)(glfwGetWindowUserPointer(window)) };

	userWindow.setSize(width, height);
}
