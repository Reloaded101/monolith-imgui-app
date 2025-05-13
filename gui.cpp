#include "monolith.h"
#include "settings_reader.h"
#include "movement_detector.h"
#include <sstream>
#include <iomanip>

static bool g_ShowWindow = true;

void renderGui(RecoilSystem& recoilSystem) {
    // Update movement state automatically
    recoilSystem.setMovementState(MovementDetector::DetectMovement());

    // Toggle window with Insert key
    if (MovementDetector::IsInsertPressed()) {
        g_ShowWindow = !g_ShowWindow;
    }

    if (!g_ShowWindow) return;

    CustomStyle();

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Once);
    ImGui::Begin("Monolith.wtf", nullptr, 
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoResize);

    // Header with logo and keybinds
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    // Logo placeholder - commented out for now as we don't have the texture
    // ImGui::Image((ImTextureID)0, ImVec2(30, 30));
    ImGui::Text("Monolith.wtf");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    if (ImGui::Button("KEYBINDS →")) {
        // TODO: Show keybinds window
    }

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.45f);

    // Left column
    ImGui::Text("WEAPON");
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
    static const char* currentWeapon = "None";
    if (ImGui::BeginCombo("##weapon", currentWeapon)) {
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Text("BARREL ATTACHMENT");
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
    static const char* currentBarrel = "None";
    if (ImGui::BeginCombo("##barrel", currentBarrel)) {
        ImGui::EndCombo();
    }
    ImGui::PopStyleColor();

    ImGui::Text("SCOPE ATTACHMENT");
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
    static const char* currentScope = "None";
    if (ImGui::BeginCombo("##scope", currentScope)) {
        ImGui::EndCombo();
    }
    ImGui::PopStyleColor();

    // Stats row
    ImGui::Text("SENSITIVITY   ADS   FOV");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
    ImGui::SetNextItemWidth(60);
    static float sensitivity = 0.010f;
    ImGui::InputFloat("##sens", &sensitivity, 0.0f, 0.0f, "%.3f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    static float ads = 0.000f;
    ImGui::InputFloat("##ads", &ads, 0.0f, 0.0f, "%.3f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60);
    static float fov = 75.0f;
    ImGui::InputFloat("##fov", &fov, 0.0f, 0.0f, "%.0f");
    ImGui::PopStyleVar();

    // Next column
    ImGui::NextColumn();

    // Right column
    ImGui::Text("SMOOTHNESS");
    static float smoothness = 0.0f;
    ImGui::SliderFloat("##smoothness", &smoothness, 0.0f, 1.0f, "%.0f");

    ImGui::Text("RANDOMIZATION");
    static float randX = 0.0f;
    ImGui::SliderFloat("##randx", &randX, 0.0f, 1.0f, "%.0f");
    ImGui::SameLine();
    ImGui::Text("X");
    static float randY = 0.0f;
    ImGui::SliderFloat("##randy", &randY, 0.0f, 1.0f, "%.0f");
    ImGui::SameLine();
    ImGui::Text("Y");

    ImGui::Text("RECOIL CONTROL AMOUNT");
    static float recoilX = 100.0f;
    ImGui::SliderFloat("##recoilx", &recoilX, 0.0f, 100.0f, "%.0f");
    ImGui::SameLine();
    ImGui::Text("X");
    static float recoilY = 100.0f;
    ImGui::SliderFloat("##recoily", &recoilY, 0.0f, 100.0f, "%.0f");
    ImGui::SameLine();
    ImGui::Text("Y");

    ImGui::Columns(1);

    // Bottom checkboxes
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 120);
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.8f, 0.4f, 0.0f, 1.0f));

    static bool oldRecoil = false;
    ImGui::Checkbox("old recoil", &oldRecoil);
    ImGui::SameLine(200);
    static bool controlHipfire = false;
    ImGui::Checkbox("control hipfire", &controlHipfire);

    static bool attachmentsNone = false;
    ImGui::Checkbox("attachments none on change", &attachmentsNone);
    ImGui::SameLine(200);
    static bool autoFire = false;
    ImGui::Checkbox("auto fire", &autoFire);

    static bool hideTask = false;
    ImGui::Checkbox("hide task on taskbar", &hideTask);
    ImGui::SameLine(200);
    static bool antiAfk = false;
    ImGui::Checkbox("anti afk", &antiAfk);

    static bool geforceNow = false;
    ImGui::Checkbox("geforce now", &geforceNow);
    ImGui::SameLine(200);
    static bool autoScope = false;
    ImGui::Checkbox("auto scope detection", &autoScope);

    static bool extendedMag = false;
    ImGui::Checkbox("extended magazine", &extendedMag);
    ImGui::SameLine(200);
    static bool autoWeapon = false;
    ImGui::Checkbox("auto weapon detection", &autoWeapon);

    ImGui::PopStyleColor(2);
    ImGui::End();
}
