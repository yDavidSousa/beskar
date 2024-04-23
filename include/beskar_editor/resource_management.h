#ifndef BESKAR_EDITOR_RESOURCE_MANAGEMENT_H
#define BESKAR_EDITOR_RESOURCE_MANAGEMENT_H

#include "editor/imgui/imgui.h"
#include "beskar_editor/texture_inspector.h"
#include <map>

std::map<resource_type, const char*> resource_type_to_string = {
        {resource_type::TEXTURE, "TEXTURE"},
        {resource_type::SHADER, "SHADER"}
};

class resource_management
{
public:
    resource_management(resource_system* resource_system)
    {
        _resource_system = resource_system;
    };

    void draw(const char* title, bool* p_open)
    {
        //ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(title, p_open) == false)
        {
            ImGui::End();
            return;
        }

        static bool show_inspector_panel = false;

        static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;
        if(ImGui::BeginTable("###table1", 6, flags))
        {
            std::vector<unsigned long long> uids = _resource_system->get_guids();

            ImGui::TableSetupColumn("UID", ImGuiTableColumnFlags_None, 0.0f, 0);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None, 0.0f, 1);
            ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_None, 0.0f, 2);
            ImGui::TableSetupColumn("References", ImGuiTableColumnFlags_None, 0.0f, 3);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None, 0.0f, 4);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 0.0f, 5);
            ImGui::TableHeadersRow();

            for (int row = 0; row < uids.size(); row++)
            {
                unsigned long long uid = uids[row];

                ImGui::TableNextRow();

                resource_metadata resource = _resource_system->lookup_resource(uid);

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%llu", resource.uid);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(resource.name.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(resource.relative_path.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%d", resource.refs_count);

                ImGui::TableSetColumnIndex(4);
                ImGui::TextUnformatted(resource_type_to_string[resource.type]);

                ImGui::TableSetColumnIndex(5);

                if(ImGui::SmallButton("View"))
                {
                    show_inspector_panel = true;
                }

                ImGui::SameLine();
                ImGui::SmallButton("Edit");
            }
            ImGui::EndTable();
        }

        if(show_inspector_panel)
        {
            static texture_inspector p_texture_inspector;
            p_texture_inspector.draw("Texture Inspector", &show_inspector_panel);
        }

        ImGui::End();
    };
private:
    resource_system* _resource_system;
};

#endif //BESKAR_EDITOR_RESOURCE_MANAGEMENT_H
