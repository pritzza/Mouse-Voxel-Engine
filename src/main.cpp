#include "Application.h"

int main()
{
    Application::instantiateLibraries();

    Application application{ "Mouse Voxel Engine", 1920 >> 0, 1080 >> 0 };

    application.start();

    Application::uninstantiateLibraries();

    return 0;
}