#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "gfx/VertexAttributes.h"

#include "util/Math.h"

#include "gfx/Materials.h"

// cube
static const std::vector<Position> cubePosVertices = {

    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },

    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },

    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },

    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },

    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },

    { -0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
};

static const std::vector<TextureCoordinate2D> cubeTexCoords = {

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f }
};

static const std::vector<Normal> cubeNormals = {
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },

   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },

   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },

   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },

   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },

   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
};

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
    {
        // allocate gl objects now that everything is loaded
        gl = std::make_unique<OpenGLStuff>();

        gl->lightSourceCube.defineAttribute(cubePosVertices);

        gl->objectCube.defineAttribute(cubePosVertices);
        gl->objectCube.defineAttribute(cubeNormals);

        VoxelSpace vs;

        const glm::ivec3 dim{ 4,4,4 };
        vs.setDimensions(dim);

        const int size{ dim.x * dim.y * dim.z };

        //for (int i = 0; i < size; ++i)
        //    if (i % 7 == 0)
        //        vs.setVoxel(i, Voxel{ Voxel::ID::Solid, 1,1,1 });

        resMang.load("a", vs);

        vs.setDimensions(dim * 4);

        resMang.load("b", vs);

        gl->voxelChunkTransform.setPosition({ 0,-5,2 });
        gl->voxelChunkTransform.update();

        gl->rayIntersectionVisualTransform.setPosition({ -20,0,0 });
        gl->rayIntersectionVisualTransform.update();

        //// shader stuff

        // object shader
        constexpr std::string_view OBJECT_VERTEX_SHADER_PATH{
            "res/shaders/Lighting/ObjectVertex.glsl"
        };
        constexpr std::string_view OBJECT_FRAGMENT_SHADER_PATH{
            "res/shaders/Lighting/ObjectFragment.glsl"
        };

        const std::vector<std::string_view> OBJECT_PROGRAM_UNIFORM_NAMES = {

            // material
            UNIFORM_MATERIAL_SHININESS,
            UNIFORM_MATERIAL_AMBIENT,
            UNIFORM_MATERIAL_DIFFUSE,
            UNIFORM_MATERIAL_SPECULAR,

            // light
            UNIFORM_LIGHT_POSITION,
            UNIFORM_LIGHT_AMBIENT,
            UNIFORM_LIGHT_DIFFUSE,
            UNIFORM_LIGHT_SPECULAR,

            // for specular lighting
            UNIFORM_VIEW_POSITION,

            // camera transform
            UNIFORM_MODEL_MAT,
            UNIFORM_VIEW_MAT,
            UNIFORM_PERSPECTIVE_MAT,
        };

        gl->objectProgram.init(
            OBJECT_VERTEX_SHADER_PATH,
            std::nullopt,
            OBJECT_FRAGMENT_SHADER_PATH,
            OBJECT_PROGRAM_UNIFORM_NAMES
        );


        /// light source shader
        constexpr std::string_view LIGHT_SOURCE_VERTEX_SHADER_PATH{
            "res/shaders/Lighting/LightSourceVertex.glsl"
        };
        constexpr std::string_view LIGHT_SOURCE_FRAGMENT_SHADER_PATH{
            "res/shaders/Lighting/LightSourceFragment.glsl"
        };

        const std::vector<std::string_view> LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES = {

            // color of light source model
            UNIFORM_LIGHT_COLOR,

            // for camera transform
            UNIFORM_MODEL_MAT,
            UNIFORM_VIEW_MAT,
            UNIFORM_PERSPECTIVE_MAT,
        };

        gl->lightSourceProgram.init(
            LIGHT_SOURCE_VERTEX_SHADER_PATH,
            std::nullopt,
            LIGHT_SOURCE_FRAGMENT_SHADER_PATH,
            LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES
        );


        /// initializing camera
        // create perspective projection matrix
        const float FOV{ glm::radians(45.f) };
        const float ASPECT_RATIO{ (float)window.getWidth() / window.getHeight() };
        const float NEAR_PLANE{ 0.1f };
        const float FAR_PLANE{ 100.0f };

        camera.setProjectionMatrix(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

        camera.setPosition({ 0.0f, 0.f, -10.f });
        camera.setYaw(glm::radians(90.f));

        // initializing object
        gl->objectTransform.scale(0.8f);

        // initializing light
        gl->lightSourceTransform.setPosition({ -4.f, -2.f, -3.f });

        gl->lightSourceTransform.update();

        gl->light.position = gl->lightSourceTransform.getPosition();
        gl->light.ambientColor = glm::vec3{ 0.1f };
    }



    // new voxel stuff
    if (USE_NEW_VOXELS)
    {
        // allocate gl objects now that everything is loaded
        gl2 = std::make_unique<OpenGLStuff2>();

        std::vector<Position> voxelPos;
        std::vector<Color> voxelColor;
        std::vector<VoxelFaces> voxelFaces;

        const float total = std::pow(6, 2);
        for (int i = 0; i < total; ++i) 
        {
            const int x = i % 6;
            const int y = i / 6;
            Position pos = { 2.f * x, 2.f * y, 0.f };
            Color color = { i / total , (i * 2 % (int)total) / total, (i * 3 % (int)total) / total, 1.f };
            VoxelFaces faces = { i };

            voxelPos.push_back(pos);
            voxelColor.push_back(color);
            voxelFaces.push_back(faces);

            pos.z += 5;
            faces.faces = 0b111111;
            voxelPos.push_back(pos);
            voxelColor.push_back(color);
            voxelFaces.push_back(faces);
        }

        gl2->voxels.defineAttribute(voxelPos);
        gl2->voxels.defineAttribute(voxelColor);
        gl2->voxels.defineAttribute(voxelFaces);

        gl2->voxelsTransform.setPosition({ 0,0,0 });
        gl2->voxelsTransform.update();

        //// shader stuff

        // object shader
        constexpr std::string_view VOXEL_VERTEX_SHADER_PATH{
            "res/shaders/VoxelVertex.glsl"
        };
        constexpr std::string_view VOXEL_GEOMETRY_SHADER_PATH{
            "res/shaders/VoxelGeometry.glsl"
        };
        constexpr std::string_view VOXEL_FRAGMENT_SHADER_PATH{
            "res/shaders/VoxelFragment.glsl"
        };

        const std::vector<std::string_view> VOXEL_PROGRAM_UNIFORM_NAMES = {
            // camera transform
            UNIFORM_MODEL_MAT,
            UNIFORM_VIEW_MAT,
            UNIFORM_PERSPECTIVE_MAT,
        };

        gl2->voxelProgram.init(
            VOXEL_VERTEX_SHADER_PATH,
            VOXEL_GEOMETRY_SHADER_PATH,
            VOXEL_FRAGMENT_SHADER_PATH,
            VOXEL_PROGRAM_UNIFORM_NAMES
        );
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

        gl->pointMatrices.push_back(m);
    }
}

void Application::update()
{
    if (window.shouldClose())
        stop();

    {
        //std::cout << "dt: " << deltaTime << '\n';

        // voxel stuff
        VoxelSpace vs{ resMang.getVoxelSpace("a") };

        static int index{ 0 };
        index++;

        const int voxelIndex{ index % vs.getSize() };

        const Voxel currentVoxel{ vs.getVoxel(voxelIndex) };

        // next voxel is inverse of current voxel
        Voxel::ID nextVoxelID{
            currentVoxel.id == Voxel::ID::Solid ? Voxel::ID::Empty : Voxel::ID::Solid
        };

        const Voxel nextVoxel{
            nextVoxelID,
            (std::sinf(currentTime * 3) / 2.f) + 0.5f,
            (std::sinf(currentTime * 5) / 2.f) + 0.5f,
            (std::sinf(currentTime * 7) / 2.f) + 0.5f
        };

        vs.setVoxel(voxelIndex, nextVoxel);

        resMang.load("a", vs);

        /// ray visual
        // visualize player direction
        vs = resMang.getVoxelSpace("b");

        for (int i = 0; i < vs.getSize(); ++i)
            vs.setVoxel(i, Voxel{ Voxel::ID::Empty, 1, 1, 1 });

        const std::vector<Math::CubeIntersection> intersections{
            Math::getRayCubeIntersections(
                glm::vec3{sin(currentTime), cos(currentTime * 2), sin(currentTime * 3)},
                //camera.getForwardDirection(),   // ray dir
                glm::vec3{ 0,0,0 },             // ray starting pos
                7                               // ray cast distance
            )
        };

        const glm::vec3 voxelPosOffset{ (vs.getDimensions() / 2) };

        const int numIntersections = intersections.size();

        for (int i = 0; i < numIntersections; ++i)
        {
            const Math::CubeIntersection& intersection{ intersections[i] };

            const float compColor{ static_cast<float>(i) / numIntersections };

            const Voxel v{ Voxel::ID::Solid, compColor, compColor, compColor };

            vs.setVoxel(intersection.cubeCoords + voxelPosOffset, v);
        }

        resMang.load("b", vs);

        // update camera matrix
        camera.update();

        // update object
        gl->objectTransform.rotate({
            deltaTime / 1.f,
            deltaTime / 2.f,
            deltaTime / 3.f
            });

        // update lighting
        if (currentTime > 20)   // wait a little before light changes colors
        {
            gl->light.diffuseColor = glm::vec3{
                1.0f,                          // r
                (sin(currentTime) / 2) + .5 ,    // g
                (cos(currentTime) / 2) + .5      // b
            };
        }

        //// pre rendering frame
        // clear color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// rendering light source

        // activate shader
        gl->lightSourceProgram.use();

        /// update all uniforms (CAMERA TRANSFORM and light color)
        // camera transform
        gl->lightSourceProgram.setUniformMat4(
            UNIFORM_MODEL_MAT,
            gl->lightSourceTransform.getMatrix()
        );
        gl->lightSourceProgram.setUniformMat4(
            UNIFORM_VIEW_MAT,
            camera.getViewMatrix()
        );
        gl->lightSourceProgram.setUniformMat4(
            UNIFORM_PERSPECTIVE_MAT,
            camera.getProjectionMatrix()
        );

        // lighting stuff
        gl->lightSourceProgram.setUniformVec3f(
            UNIFORM_LIGHT_COLOR,
            gl->light.diffuseColor
        );

        // bind light source VAO
        gl->lightSourceCube.bind();

        // draw light source
        glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());


        /// rendering object

        // activate object shader
        gl->objectProgram.use();

        const int cubesLength{ 1 };
        for (int i = 0; i < cubesLength * cubesLength; ++i)
        {
            const float xPos = (i % cubesLength) - (cubesLength / 2.f);
            const float yPos = (i / cubesLength) - (cubesLength / 2.f);
            const float zPos = sqrt(pow(xPos, 2) + pow(yPos, 2)) + cubesLength / 2.f;

            const glm::vec3 lightPos{ gl->lightSourceTransform.getPosition() };

            gl->objectTransform.setPosition({
                xPos,
                yPos + lightPos.y,
                zPos
                });

            gl->objectTransform.update();

            // setting object material based on index
            static constexpr int NUM_MATERIALS{ static_cast<int>(Materials::ID::SIZE) };
            Materials::ID matID{ static_cast<Materials::ID>(i % NUM_MATERIALS) };

            if (i == 0)
                matID = Materials::ID::Pearl;

            gl->objectMaterial = Materials::MATERIALS.getVal(matID);


            /// update all uniforms:
            /// camera transform, view position, light, and object material

            // camera transform
            gl->objectProgram.setUniformMat4(
                UNIFORM_MODEL_MAT,
                gl->objectTransform.getMatrix()
            );
            gl->objectProgram.setUniformMat4(
                UNIFORM_VIEW_MAT,
                camera.getViewMatrix()
            );
            gl->objectProgram.setUniformMat4(
                UNIFORM_PERSPECTIVE_MAT,
                camera.getProjectionMatrix()
            );

            // lighting stuff (view position, light, object material)
            gl->objectProgram.setUniformVec3f(
                UNIFORM_VIEW_POSITION,
                camera.getPosition()
            );

            gl->objectProgram.setUniformLight(
                UNIFORM_LIGHT_POSITION,
                UNIFORM_LIGHT_AMBIENT,
                UNIFORM_LIGHT_DIFFUSE,
                UNIFORM_LIGHT_SPECULAR,
                gl->light
            );

            gl->objectProgram.setUniformMaterial(
                UNIFORM_MATERIAL_SHININESS,
                UNIFORM_MATERIAL_AMBIENT,
                UNIFORM_MATERIAL_DIFFUSE,
                UNIFORM_MATERIAL_SPECULAR,
                gl->objectMaterial
            );

            // bind object VAO
            gl->objectCube.bind();

            // draw object
            glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());
        }

        for (const ModelMatrix& pointMatrix : gl->pointMatrices)
        {
            gl->objectProgram.setUniformMat4(
                UNIFORM_MODEL_MAT,
                pointMatrix.getMatrix()
            );

            gl->objectCube.bind();

            glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());
        }

        gl->objectProgram.setUniformMat4(
            UNIFORM_MODEL_MAT,
            gl->voxelChunkTransform.getMatrix()
        );

        const VAO& voxelVAO{ resMang.getVoxelMeshVAO("a") };
        voxelVAO.bind();

        glDrawArrays(GL_TRIANGLES, 0, voxelVAO.getNumVertices());

        gl->objectProgram.setUniformMat4(
            UNIFORM_MODEL_MAT,
            gl->rayIntersectionVisualTransform.getMatrix()
        );

        const VAO& rayIntersectionVisualVAO{ resMang.getVoxelMeshVAO("b") };
        rayIntersectionVisualVAO.bind();

        glDrawArrays(GL_TRIANGLES, 0, rayIntersectionVisualVAO.getNumVertices());

    }

    if (USE_NEW_VOXELS)
    {
        gl2->voxelProgram.use();

        gl2->voxelProgram.setUniformMat4(
            UNIFORM_MODEL_MAT,
            gl2->voxelsTransform.getMatrix()
        );
        gl2->voxelProgram.setUniformMat4(
            UNIFORM_VIEW_MAT,
            camera.getViewMatrix()
        );
        gl2->voxelProgram.setUniformMat4(
            UNIFORM_PERSPECTIVE_MAT,
            camera.getProjectionMatrix()
        );
        
        gl2->voxels.bind();

        glDrawArrays(GL_POINTS, 0, gl2->voxels.getNumVertices());
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