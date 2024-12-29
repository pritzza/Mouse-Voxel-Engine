#include "Application.h"

#include "../tests/Tests.h"

int main()
{
    if (!runTests())
        return 0;

    Application::instantiateLibraries();

    Application application{ "Mouse Voxel Engine", 1920/2 >> 0, 1080/2 >> 0 };

    application.start();

    Application::uninstantiateLibraries();

    return 0;
}