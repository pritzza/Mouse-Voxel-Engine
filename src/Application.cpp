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
    const float fov = glm::radians(90.f);
    const float aspectRatio = (float)window.getWidth() / window.getHeight();
    const float nearPlane = 0.1f;
    const float farPlane = 1000.f;
    
    camera.setProjectionMatrix(
        fov,
        aspectRatio,
        nearPlane,
        farPlane
    );

    camera.setPosition({ 0.f, 0.f, 0.f });
    camera.setYaw(glm::radians(90.f));

    // allocate gl objects now that everything is loaded
    gl = std::make_unique<OpenGLStuff2>();

    {
        gl->grid = std::make_shared<VoxelGrid>(glm::ivec3{ 8,8,8 });
        gl->model = std::make_shared<VoxelModel>();

        VoxelGrid& g{ *gl->grid.get() };
        const float size = g.getSize();

        for (int i = 0; i < size; ++i)
        {
            Color color = { i / size , (i * 2 % (int)size) / size, (i * 3 % (int)size) / size, 1.f };
            Voxel voxel{ Voxel::ID::Filled, color };
            g.setVoxel(i, voxel);
        }

        VoxelModel& m{ *gl->model.get() };
        m.create(gl->grid);

        VoxelObject& o{ gl->object };
        o.model = gl->model;

        o.transform.setPosition({ 10,0,0 });
        o.transform.update();
    }

    // make big red, green, and blue models
    {
        const glm::ivec3 dim{ 8,8,8 };
        for (int i = 0; i < 3; ++i)
        {
            // create a grid
            std::shared_ptr<VoxelGrid> g{ std::make_shared<VoxelGrid>(dim * (i + 1)) };

            for (int j = 0; j < g->getSize(); ++j)
            {
                // ignore this horrific code of me picking color and ID
                const float f = 4.0 * j / g->getSize();
                const Voxel::ID id{ (j % (i + 2)) == 0 ? Voxel::ID::Filled : Voxel::ID::Null };
                const Color c((i == 0) * f, (i == 1) * f, (i == 2) * f, 1);

                const Voxel v{ id, c };
                g->setVoxel(j, v);
            }

            // add grid to resources
            grids.set(i, g);

            // create two models
            std::shared_ptr<VoxelModel> m1{ std::make_shared<VoxelModel>(grids.get(i)) };
            std::shared_ptr<VoxelModel> m2{ std::make_shared<VoxelModel>(grids.get(i)) };

            // save it one to resources
            models.set(i, m1);

            // get grid from resources
            g = grids.get(i);

            // change grid that both models use
            const Voxel v2{ Voxel::ID::Filled, Color(1,1,1,1) };
            g->setVoxel(0, v2);

            // sync second model to updated grid
            m2->syncToGrid();

            // add second model to resources
            models.set(i + 3, m2);

            // now grids shoud look like:
            // 0 -> red updated
            // 1 -> green updated
            // 2 -> blue updated

            // now models shoud look like:
            // 0 -> red old
            // 1 -> green old
            // 2 -> blue old
            // 3 -> red updated
            // 4 -> green updated
            // 5 -> blue updated
        }
    }

    // create a white 2x2x2 voxel model/grid at id[ -1 ]
    {
        std::shared_ptr<VoxelGrid> g = std::make_shared<VoxelGrid>(glm::ivec3{ 2,2,2 });

        for (int i = 0; i < g->getSize(); ++i)
        {
            Color color = { 1.f, 1.f, 1.f, 1.f };
            Voxel voxel{ Voxel::ID::Filled, color };
            g->setVoxel(i, voxel);
        }

        g->ammendAlterations();

        std::shared_ptr<VoxelModel> m = std::make_shared<VoxelModel>();

        m->create(g);

        models.set(-1, m);
        grids.set(-1, g);
    }

    {
        for (int i = 0; i < 3; ++i)
        {
            std::shared_ptr<VoxelGrid> g = std::make_shared<VoxelGrid>(glm::ivec3{ 2,2,2 });
            for (int j = 0; j < g->getSize(); ++j)
            {
                Color color(i == 0, i == 1, i == 2, 1);
                Voxel voxel{ Voxel::ID::Filled, color };
                g->setVoxel(j, voxel);
            }

            std::shared_ptr<VoxelModel> m = std::make_shared<VoxelModel>();
            m->create(g);

            const int id = i + 100;
            models.set(id, m);
            grids.set(id, g);
        }
    }

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
        camera.moveAbsolute(Math::J_HAT * travelDistance);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveAbsolute(-Math::J_HAT * travelDistance);

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
        // TODO mouse picking
        glm::ivec2 windowDim{ window.getWidth(), window.getHeight() };

        // mouse position from center of window within interval [-1, 1]
        glm::dvec2 mousePos;
        glfwGetCursorPos(window.getWindowPtr(), &mousePos.x, &mousePos.y);
        mousePos -= windowDim / 2;    // get 0,0 to be center of window
        mousePos /= windowDim / 2;    // get 1,1 to be bottom right of window

        std::cout << mousePos.x << ", " << mousePos.y << '\n';

        const float fov = camera.getFOV();
        const float aspectRatio = camera.getAspectRatio();

        float mousePitch = -fov/2 * mousePos.y;
        float mouseYaw = fov/2 * mousePos.x * aspectRatio;


        const glm::vec3& camPos{ camera.getPosition() };
        const float camReach{ 5.f };

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

    // update voxel stuff
    VoxelGrid& vg = *gl->grid.get();
    const glm::ivec3& dim = vg.getDim();;
    const float size = vg.getSize();
    const int t = currentTime * 10;

    // before potentially changing something, prep state to detect changes
    vg.ammendAlterations();
    
    if (currentTime > 3)
    {
        //// make 2 random voxels empty
        for (int i = 0; i < 2; ++i)
        {
            const int randomIndex = Math::rng(0, size);
            //vg.setID(randomIndex, Voxel::ID::Null);
            Voxel v{ Voxel::ID::Null, Color(.5,.5,.5,1) };
            vg.setVoxel(randomIndex, v);
        }
    
        // make 1 rancom voxel filled
        for (int i = 0; i < 1; ++i)
        {
            Voxel v{ Voxel::ID::Filled, Color(1,1,1,1) };

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

    gl->model->syncToGrid();
    
    gl->shader.update(
        camera.getViewMatrix(),
        camera.getProjectionMatrix(),
        camera.getPosition(),
        currentTime
    );

    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //// rendering frame
    gl->renderer.render(gl->object, gl->shader);

    for (int i = 0; i < 6; ++i)
    {
        VoxelObject o;
        o.model = models.get(i);
        o.transform.setPosition(glm::vec3( (i * 32) % (80), (i>2) * 32, 16));
        o.transform.update();

        gl->renderer.render(o, gl->shader);
    }

    {
        VoxelObject o;
        o.model = models.get(-1);
        gl->renderer.render(o, gl->shader);
    }

    for (int i = 0; i < 3; ++i)
    {
        const int id = i + 100;

        VoxelObject o;
        o.model = models.get(id);
        o.transform.setPosition(glm::vec3(i == 0, i == 1, i == 2) * 8.f);
        o.transform.update();

        gl->renderer.render(o, gl->shader);
    }
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