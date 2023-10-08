#pragma once
#include "imgui.h"
#include "global.h"




void info_window(bool* open) {

}




void control_window(bool* open) {
    ImGui::Begin("Current Points");
    
    ImGui::InputFloat2("Point", curPoint);
    if (ImGui::Button("Save")) {

    }
    
    ImGui::End();
}