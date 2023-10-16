#pragma once
#include "imgui.h"
#include "HM1.h"
#include <algorithm>




void info_window(bool* open) {
    if (*open) {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Points Information", open, windowFlags);
        for (size_t i = 0; i < HM1::controlPoints.size(); ++i) {
            ImGui::Text("Point %d : (%f, %f)", i, HM1::controlPoints[i].x(), HM1::controlPoints[i].y());
        }
        ImGui::End();
    }
}




void control_window(bool* open) {
    if (*open) {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Current Points", open, windowFlags);
        if (ImGui::Button("Clear")) {
            HM1::controlPoints.clear();
            HM1::resultGauss.clear();
            HM1::resultPolynomial.clear();
            HM1::resultLeastSquare.clear();
            HM1::resultRidge.clear();
        }
    
        ImGui::End();
    }
    
}
