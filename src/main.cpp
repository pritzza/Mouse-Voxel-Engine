#include "Application.h"

int main()
{
    Application::instantiateLibraries();

    Application application{ "Finally", 800, 600 };

    application.start();

    Application::uninstantiateLibraries();

    return 0;
}