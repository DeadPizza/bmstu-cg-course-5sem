#include "application/Application.h"

int main()
{
    std::unique_ptr<Application> app = std::make_unique<Application>();

    app->Init();
    app->Run();
    
    return 0;
}