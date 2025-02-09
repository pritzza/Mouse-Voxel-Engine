#pragma once

#include <glad/glad.h>  // remember to include glad or glad debug dir in proj
#include <GLFW/glfw3.h>

#include "gfx/Camera.h"
#include "gfx/Window.h"

#include "util/ResourceManager.h"

#include "voxel/VoxelGrid.h"

#include "gfx/VoxelModel.h"

#include "gfx/MainPassVoxelShader.h"
#include "gfx/ShadowPassVoxelShader.h"

#include "gfx/VoxelRenderer.h"
#include "gfx/VoxelObject.h"

#include "gfx/Background.h"

class FBO;

class Application
{
public:
	static void instantiateLibraries();
	static void uninstantiateLibraries();

public:
	Application(
		const std::string_view& name,
		int windowWidth,
		int windowHeight
	);

	void start();
	void stop();

public:
	bool isRunning{ false };
	
	double currentTime{ 0.0 };
	double deltaTime{ 0.0 };

	Window window;
	Camera camera;

	ResourceManager<VoxelGrid> grids;
	ResourceManager<VoxelModel> models;

	int chunkScale{ 1 };

	struct OpenGLStuff2
	{
		Background bg;

		// todo, these shouldnt need to be smart ptrs, im just lazy
		std::shared_ptr<FBO> fbo;
		std::shared_ptr<FBO> db;

		std::shared_ptr<VoxelGrid> grid;
		std::shared_ptr<VoxelModel> model;

		VoxelObject object;

		MainPassVoxelShader mainPass;
		ShadowPassVoxelShader shadowPass;

		VoxelRenderer renderer;
	};

	std::unique_ptr<OpenGLStuff2> gl;

private:
	static constexpr int OPENGL_MAJOR_VERSION{ 3 };
	static constexpr int OPENGL_MINOR_VERSION{ 3 };
	static constexpr int OPENGL_PROFILE{ GLFW_OPENGL_CORE_PROFILE };

private:
	void initializeObjects();

	void applicationLoop();

	void handleInput();
	void update();
	void display();

	// static methods for library initialization and configuration
private:
	static inline bool isInstantiated{ false };

	static void initGLFW();
	static void initGLAD();
	static void initOpenGL();
	static void initSTB_Image();
};