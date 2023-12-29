#include "Application.h"

int main()
{
    Application::instantiateLibraries();

    Application application{ "Mouse Voxel Engine", 1920 >> 1, 1080 >> 1 };

    application.start();

    Application::uninstantiateLibraries();

    return 0;
}