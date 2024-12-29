#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "util/PerlinNoise.hpp"

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

    if (true)
    {

        gl->grid = std::make_shared<VoxelGrid>(glm::ivec3{ 8,8,8 });
        gl->model = std::make_shared<VoxelModel>();

        VoxelGrid& g{ *gl->grid.get() };
        const float size = g.getSize();

        for (int i = 0; i < size; ++i)
        {
            VoxelGraphicsData color{
                i / size , 
                (i * 2 % (int)size) / size, 
                (i * 3 % (int)size) / size, 
                1.f 
            };
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

    // create a white 2x2x2 voxel model/grid at id[ -1 ]
    {
        std::shared_ptr<VoxelGrid> g = std::make_shared<VoxelGrid>(glm::ivec3{ 3,3,3 });

        for (int i = 0; i < g->getSize(); ++i)
        {
            VoxelGraphicsData color = { 1.f, 1.f, 1.f, 1.f };
            Voxel voxel{ Voxel::ID::Filled, color };
            g->setVoxel(i, voxel);
        }

        std::shared_ptr<VoxelModel> m = std::make_shared<VoxelModel>();
        m->create(g);
        models.set(1000, m);

        for (int i = 0; i < 9; ++i)
        {
            const glm::ivec3 coord = glm::ivec3{ 0,2,0 } + Math::toCoord(i, { 3,1,3 });
            g->setID(coord, Voxel::ID::Null);
            m = std::make_shared<VoxelModel>();
            m->create(g);
            models.set(1001 + i, m);
        }
    }

    if (true)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::shared_ptr<VoxelGrid> g = std::make_shared<VoxelGrid>(glm::ivec3{ 2,2,2 });
            for (int j = 0; j < g->getSize(); ++j)
            {
                VoxelGraphicsData color(i == 0, i == 1, i == 2, 1);
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

    // making 64x32x64 terrain chunk
    {
        const glm::ivec3 chunkDim{ 256,64,256 };
        std::shared_ptr<VoxelGrid> chunk{ std::make_shared<VoxelGrid>(chunkDim) };
        const int chunkSize{ chunkDim.x * chunkDim.y * chunkDim.z };
        for (int i = 0; i < chunkSize; ++i)
            chunk->setID(i, Voxel::ID::Null);

        int seed = 0;
        float frequency = 4;
        float octaves = 4;
        const siv::PerlinNoise perlin( seed );
        const double fx = (frequency / chunkDim.x);
        const double fz = (frequency / chunkDim.z);

        for (int z = 0; z < chunkDim.z; ++z)
            for (int x = 0; x < chunkDim.x; ++x)
            {
                const float heightNormalized = perlin.octave2D_01(x * fx, z * fz, octaves);
                const float height{ heightNormalized * chunkDim.y };
                for (int i = 0; i < height; ++i)
                {
                    const VoxelGraphicsData SNOW{  0.95, 0.95, 1.0,  1.0 };
                    const VoxelGraphicsData WATER{ 0.1,  0.1,  0.9,  1.0 };
                    const VoxelGraphicsData SAND{  0.8,  0.8,  0.2,  1.0 };
                    const VoxelGraphicsData STONE{ 0.6,  0.6,  0.6,  1.0 };
                    const VoxelGraphicsData GRASS{ 0.6,  0.9,  0.3,  1.0 };

                    VoxelGraphicsData color;
                    if (heightNormalized > .8)
                        color = SNOW;
                    else if (heightNormalized > .65)
                        color = STONE;
                    else if (heightNormalized > .3)
                        color = GRASS;
                    else if (heightNormalized > .25)
                        color = SAND;
                    else
                        color = WATER;

                    const Voxel v{ Voxel::ID::Filled, color };
                    
                    const glm::ivec3 coord{ x, i, z };
                    chunk->setVoxel(coord, v);
                }
            }
        chunk->ammendAlterations();
        std::shared_ptr<VoxelModel> m = std::make_shared<VoxelModel>();
        m->create(chunk);

        const int id = 10000;
        models.set(id, m);
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

    if (true)
    {




        // update voxel stuff
        VoxelGrid& vg = *gl->grid.get();
        const glm::ivec3& dim = vg.getDim();;
        const float size = vg.getSize();
        const int t = currentTime * 10;

        // before potentially changing something, prep state to detect changes
        vg.ammendAlterations();

        //// make 2 random voxels empty
        for (int i = 0; i < 2; ++i)
        {
            const int randomIndex = Math::rng(0, size);
            //vg.setID(randomIndex, Voxel::ID::Null);
            Voxel v{ Voxel::ID::Null, VoxelGraphicsData(.5,.5,.5,1) };
            vg.setVoxel(randomIndex, v);
        }

        // make 1 random voxel filled
        for (int i = 0; i < 1; ++i)
        {
            Voxel v{ Voxel::ID::Filled, VoxelGraphicsData(1,1,1,1) };

            const int randomIndex = Math::rng(0, size);
            vg.setID(randomIndex, Voxel::ID::Filled);
            VoxelGraphicsData color = {
                (randomIndex * 1 % (int)size) / size ,
                (randomIndex * t % (int)size) / size,
                (randomIndex * 3 % (int)size) / size,
                1.f };
            vg.setGraphic(randomIndex, color);
        }

        gl->model->syncToGrid();




    }

    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // update shader uniforms
    gl->shader.update(
        camera.getViewMatrix(),
        camera.getProjectionMatrix(),
        camera.getPosition(),
        currentTime
    );

    //// rendering frame
    gl->renderer.render(gl->object, gl->shader);

    for (int i = 0; i < 9; ++i)
    {
        VoxelObject o;
        o.model = models.get(1000 + i);
        o.transform.setPosition({ 0,0, -4 * i });
        o.transform.update();
        gl->renderer.render(o, gl->shader);
    }

    if (true)
    {
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

    {
        const int id = 10000;

        VoxelObject o;
        o.model = models.get(id);
        for (int i = 1; i <= 1; ++i)
        {
            o.transform.setPosition(glm::vec3(0, -100 * i, 0));
            o.transform.update();

            gl->renderer.render(o, gl->shader);
        }
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
    glEnable(GL_MULTISAMPLE);

	// must include this line or else program crashes upon buffering texture data
	// https://stackoverflow.com/questions/9950546/c-opengl-glteximage2d-access-violation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Application::initSTB_Image()
{
	// so textures aren't displayed upsidedown
	stbi_set_flip_vertically_on_load(true);
}