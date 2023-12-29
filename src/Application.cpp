#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "gfx/VertexAttributes.h"

#include "util/Math.h"

#include "VoxelGrid.h"

Application::Application(
	const std::string_view& name, 
	int windowWidth, 
	int windowHeight
)
	:
	window{ 
		name, 
		windowWidth, 
		windowHeight, 
		Application::OPENGL_MAJOR_VERSION, 
		Application::OPENGL_MINOR_VERSION,
		Application::OPENGL_PROFILE
	}
{
	if (!Application::isInstantiated)
	{
		std::cerr << "Error! Tried to create Application before "
			<< "instantiating libraries... How are you even seeing this?\n";

		assert(false);
	}


	// todo: turns our glad can only be loaded after creating context
	initGLAD();
	initOpenGL();
}

void Application::start()
{
	isRunning = true;

	initializeObjects();
	applicationLoop();
}

void Application::stop()
{
	isRunning = false;
	// terminate()
}

void Application::initializeObjects()
{
    /// initializing camera
    // create perspective projection matrix
    const float FOV{ glm::radians(FOV_DEG) };
    //const float ASPECT_RATIO{ (float)window.getWidth() / window.getHeight() };
    ASPECT_RATIO = (float)window.getWidth() / window.getHeight();
    //const float NEAR_PLANE{ 0.1f };
    //const float FAR_PLANE{ 1000.0f };

    camera.setProjectionMatrix(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

    camera.setPosition({ 0.0f, 0.f, -10.f });
    camera.setYaw(glm::radians(90.f));

    // allocate gl objects now that everything is loaded
    gl = std::make_unique<OpenGLStuff2>();

    gl->grid = std::make_shared<VoxelGrid>(glm::ivec3{ 8,8,8 });
    gl->model = std::make_shared<VoxelModel>();

    VoxelGrid& g{ *gl->grid.get() };
    const float size = g.getSize();

    for (int i = 0; i < size; ++i)
    {
        Color color = { i / size , (i * 2 % (int)size) / size, (i * 3 % (int)size) / size, 1.f };
        Voxel voxel{ Voxel::ID::Filled, color, 0 };
        g.set(i, voxel);
    }

    VoxelModel& m{ *gl->model.get() };
    m.create(gl->grid);

    VoxelObject& o{ gl->object };
    o.model = gl->model;

    o.transform.setPosition({ 10,0,0 });
    o.transform.update();
}

void Application::applicationLoop()
{
	while (isRunning)
	{
        deltaTime = glfwGetTime() - currentTime;
        currentTime = glfwGetTime();

		handleInput();
		update();
		display();
	}
}

void Application::handleInput()
{
    // handle input
    window.pollEvents();

    GLFWwindow* const glfwWindow{ window.getWindowPtr() };

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);

    float moveSpeed{ 1.0f };
    const float quickSpeedModifier{ 5.0f };

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        moveSpeed *= quickSpeedModifier;

    const float travelDistance = moveSpeed * deltaTime;

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Backward, travelDistance);
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Forward, travelDistance);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Left, travelDistance);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Right, travelDistance);

    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveAbsolute(Math::J_HAT, travelDistance);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveAbsolute(-Math::J_HAT, travelDistance);

    const float rotationSpeed{ 1.f };
    const float rotationAngle = rotationSpeed * deltaTime;

    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        camera.setPitch(camera.getPitch() + rotationAngle);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.setPitch(camera.getPitch() - rotationAngle);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.setYaw(camera.getYaw() - rotationAngle);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.setYaw(camera.getYaw() + rotationAngle);

    if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glm::ivec2 windowDim{ window.getWidth(), window.getHeight() };

        // mouse position from center of window within interval [-1, 1]
        glm::dvec2 mousePos;
        glfwGetCursorPos(window.getWindowPtr(), &mousePos.x, &mousePos.y);
        mousePos -= windowDim / 2;    // get 0,0 to be center of window
        mousePos /= windowDim / 2;    // get 1,1 to be bottom right of window

        std::cout << mousePos.x << ", " << mousePos.y << '\n';

        float mousePitch = -glm::radians(FOV_DEG / 2.0) * mousePos.y;
        float mouseYaw = glm::radians(FOV_DEG / 2.0) * mousePos.x * ASPECT_RATIO;


        const glm::vec3& camPos{ camera.getPosition() };
        const float camReach{ 5.f };
        const float aspectRatio{ (float)windowDim.x / windowDim.y };

        /// camera's direction coordinate system
        const glm::vec3 camFront{ camera.getForwardDirection() * camReach };
        // todo: camera FOV has something to do with this
        const glm::vec3 camRight{ camera.getRightDirection() * (float)mousePos.x * camReach };
        const glm::vec3 camUp{ camera.getUpDirection() * -(float)mousePos.y * camReach * aspectRatio };

        ModelMatrix m;
        m.setPosition(camPos + camFront + camRight + camUp);

        const float DRAWING_UNIT_SCALE{ 1.f / 10.f };
        m.scale(DRAWING_UNIT_SCALE);

        m.update();

        //gl->pointMatrices.push_back(m);
    }

    // mouse
    const glm::ivec2 windowDim{ window.getWidth(), window.getHeight() };

    // mouse position from center of window within interval [-1, 1]
    static glm::dvec2 prevMousePos{ 0.0, 0.0 };
    static glm::dvec2 curMousePos{ 0.0, 0.0 };

    prevMousePos = curMousePos;
    glfwGetCursorPos(window.getWindowPtr(), &curMousePos.x, &curMousePos.y);
    
    const glm::dvec2 deltaMousePos{ prevMousePos - curMousePos };
    
    static constexpr double MOUSE_SENSITIVITY{ 1/128.0 };
    const glm::dvec2 pan{ deltaMousePos * MOUSE_SENSITIVITY };

    camera.setPitch(std::clamp(camera.getPitch() + pan.y, glm::radians(-89.9), glm::radians(89.9)));
    camera.setYaw(camera.getYaw() - pan.x);
}

void Application::update()
{
    if (window.shouldClose())
        stop();

    // update camera matrix
    camera.update();

    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    VoxelGrid& vg = *gl->grid.get();
    const glm::ivec3& dim = vg.getDim();;
    const float size = vg.getSize();
    const int t = currentTime * 10;
    
    if (currentTime > 3)
    {
        //// make 2 random voxels empty
        for (int i = 0; i < 2; ++i)
        {
            const int randomIndex = Math::rng(0, size);
            //vg.setID(randomIndex, Voxel::ID::Null);
            Voxel v{ Voxel::ID::Null, Color(.5,.5,.5,1), 0 };
            vg.set(randomIndex, v);
        }
    
        // make 1 rancom voxel filled
        for (int i = 0; i < 1; ++i)
        {
            Voxel v{ Voxel::ID::Filled, Color(1,1,1,1), 0 };

            const int randomIndex = Math::rng(0, size);
            vg.setID(randomIndex, Voxel::ID::Filled);
            Color color = { 
                (randomIndex * 1 % (int)size) / size ,
                (randomIndex * t % (int)size) / size, 
                (randomIndex * 3 % (int)size) / size, 
                1.f };
            vg.setGraphic(randomIndex, color);
        }
    }
    
    gl->model->updateBuffers();
    
    gl->shader.update(
        camera.getViewMatrix(),
        camera.getProjectionMatrix(),
        camera.getPosition(),
        currentTime
    );

    gl->renderer.render(gl->object, gl->shader);
}

void Application::display()
{
    window.swapBuffers();
}


///// static methods
void Application::instantiateLibraries()
{
	initGLFW();

	initSTB_Image();

	Application::isInstantiated = true;
}

void Application::uninstantiateLibraries()
{
	// pretty sure this frees GL context
	glfwTerminate();

	Application::isInstantiated = false;
}

void Application::initGLFW()
{
	glfwInit();
}

void Application::initGLAD()
{
	// load opengl functions with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Error! Failed to initialize GLAD.\n";
}

void Application::initOpenGL()
{
	// set clear color of color buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// must include this line or else program crashes upon buffering texture data
	// https://stackoverflow.com/questions/9950546/c-opengl-glteximage2d-access-violation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Application::initSTB_Image()
{
	// so textures aren't displayed upsidedown
	stbi_set_flip_vertically_on_load(true);
}