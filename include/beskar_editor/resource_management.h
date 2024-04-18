#ifndef BESKAR_EDITOR_RESOURCE_SYSTEM_P_H
#define BESKAR_EDITOR_RESOURCE_SYSTEM_P_H

#include "editor/imgui/imgui.h"

class resource_management
{
public:
    resource_management(resource_system* resource_system)
    {
        _resource_system = resource_system;
    };

    void draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title, p_open) == false)
        {
            ImGui::End();
            return;
        }

        ImGui::End();
    };
private:
    resource_system* _resource_system;
};

#endif //BESKAR_EDITOR_RESOURCE_SYSTEM_P_H
