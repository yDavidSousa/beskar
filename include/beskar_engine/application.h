#ifndef BESKAR_APPLICATION_H
#define BESKAR_APPLICATION_H

#include "../../src/editor/imgui/imgui.h"
#include <filesystem>

class application
{
public:
    application(const char* path);
    ~application();

    void initialize();
    void run(int width, int height);
    void update(float delta_time);
    void gui();

    std::filesystem::path data_path;
    std::filesystem::path persistent_data_path;
};

#endif //BESKAR_APPLICATION_H
