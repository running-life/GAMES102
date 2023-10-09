#pragma once
#include "imgui.h"
#include "global.h"




void info_window(bool* open) {
    ImGui::Begin("Points Information", open);
    for (size_t i = 0; i < controlPoints.size(); i += 5) {
        ImGui::Text("Point %d : (%f, %f)", i / 5, controlPoints[i], controlPoints[i + 1]);
    }
    ImGui::End();
}




void control_window(bool* open) {
    ImGui::Begin("Current Points", open);
    
    ImGui::InputFloat2("Point", curPoint);

    float curPointCopy[2] = { curPoint[0], curPoint[1] };

    if (ImGui::Button("Save")) {
        controlPoints.push_back(curPointCopy[0]);
        controlPoints.push_back(curPointCopy[1]);
        controlPoints.push_back(0.0f);
        controlPoints.push_back(0.0f);
        controlPoints.push_back(0.0f);
    }
    
    ImGui::End();
}