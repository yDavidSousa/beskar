#ifndef BESKAR_ENGINE_APPLICATION_H
#define BESKAR_ENGINE_APPLICATION_H

#include "beskar_engine/resource_system.h"
#include "beskar_engine/renderer_system.h"

class application
{
public:
    application(const char* path, int width, int height);
    ~application();

    void initialize();
    void run();
    void update(float delta_time);
    void gui();

    int width, height;

    resource_system* resource_system;
    renderer_system* renderer_system;
};

application* beskar_main(int argc, char** argv);

#endif //BESKAR_ENGINE_APPLICATION_H
