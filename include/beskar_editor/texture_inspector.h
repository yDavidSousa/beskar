#ifndef BESKAR_EDITOR_TEXTURE_INSPECTOR_H
#define BESKAR_EDITOR_TEXTURE_INSPECTOR_H

#include "editor/imgui/imgui.h"

class texture_inspector
{
public:
    texture_inspector()
    {
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
};

#endif //BESKAR_EDITOR_TEXTURE_INSPECTOR_H
