#ifndef PACMAN_RESOURCE_MANAGER_H
#define PACMAN_RESOURCE_MANAGER_H

#include <filesystem>

class resource_manager
{
public:
    std::filesystem::path data_path;
};

#endif //PACMAN_RESOURCE_MANAGER_H