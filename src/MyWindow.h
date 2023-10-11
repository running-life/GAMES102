#pragma once
#include "imgui.h"
#include "global.h"
#include "HM1.h"
#include <algorithm>




void info_window(bool* open) {
    if (*open) {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Points Information", open, windowFlags);
        for (size_t i = 0; i < HM1::controlPoints.size(); ++i) {
            ImGui::Text("Point %d : (%f, %f)", i, HM1::controlPoints[i].x, HM1::controlPoints[i].y);
        }
        ImGui::End();
    }
}




void control_window(bool* open) {
    if (*open) {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Current Points", open, windowFlags);
    
        ImGui::InputFloat2("Point", curPoint);

        

        if (ImGui::Button("Save")) {
            HM1Point temp(curPoint[0], curPoint[1], 0.0f, 0.0f, 0.0f);
            HM1::controlPoints.push_back(temp);
            std::sort(HM1::controlPoints.begin(), HM1::controlPoints.end(), [](HM1Point& a, HM1Point& b) {
                return a.x < b.x;
                });
            HM1::polynomialInterpolation();
            
        }
    
        ImGui::End();
    }
    
}