#ifndef BESKAR_APPLICATION_H
#define BESKAR_APPLICATION_H

#include "../../src/editor/imgui/imgui.h"
#include <filesystem>

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
    std::filesystem::path data_path;
    std::filesystem::path persistent_data_path;
};

application* beskar_main(int argc, char** argv);

#endif //BESKAR_APPLICATION_H
