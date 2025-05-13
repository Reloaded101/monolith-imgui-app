#include "monolith.h"

// Error callback
void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void CustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Main styling
    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 0.0f;
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;

    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
}

// Recoil calculation implementation
void RecoilSystem::calculateRecoil(float deltaTime, float* outX, float* outY) {
    if (!m_isActive || (!m_controlHipfire && !m_isScoped)) {
        *outX = 0.0f;
        *outY = 0.0f;
        return;
    }

    // Calculate effective sensitivity
    float effectiveSens = m_isScoped ? m_sensitivity * (1.0f - m_ads) : m_sensitivity;

    // Generate random recoil targets periodically
    static float timeSinceLastUpdate = 0.0f;
    timeSinceLastUpdate += deltaTime;
    if (timeSinceLastUpdate >= 0.1f) { // Update every 100ms
        timeSinceLastUpdate = 0.0f;

        // Random component based on randomization settings
        float randX = ((rand() % 200) - 100) * m_randomizationX / 10000.0f;
        float randY = ((rand() % 200) - 100) * m_randomizationY / 10000.0f;

        if (m_useOldRecoil) {
            // Old recoil pattern (more vertical)
            m_targetRecoilX = randX;
            m_targetRecoilY = 0.15f + randY; // Base vertical recoil + randomization
        }
        else {
            // New recoil pattern (more spread)
            m_targetRecoilX = 0.05f + randX;
            m_targetRecoilY = 0.1f + randY;
        }

        // Apply recoil control
        m_targetRecoilX *= m_recoilControlX / 100.0f;
        m_targetRecoilY *= m_recoilControlY / 100.0f;
    }

    // Smoothly interpolate current recoil towards target
    float smoothFactor = (100.0f - m_smoothness) / 100.0f;
    smoothFactor = smoothFactor * smoothFactor * deltaTime * 60.0f; // Quadratic smoothing

    m_currentRecoilX = m_currentRecoilX + (m_targetRecoilX - m_currentRecoilX) * smoothFactor;
    m_currentRecoilY = m_currentRecoilY + (m_targetRecoilY - m_currentRecoilY) * smoothFactor;

    // Output final recoil values
    *outX = m_currentRecoilX * effectiveSens;
    *outY = m_currentRecoilY * effectiveSens;
}

void renderGui(RecoilSystem& recoilSystem) {
    // Helper functions for styled widgets
    auto StyledSlider = [](const char* label, float* value, float min, float max, const char* format) {
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        bool changed = ImGui::SliderFloat(label, value, min, max, format);
        ImGui::PopStyleColor(2);
        return changed;
        };

    auto StyledCombo = [](const char* label, int* current_item, const char* const items[], int items_count) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
        bool changed = ImGui::Combo(label, current_item, items, items_count);
        ImGui::PopStyleColor(5);
        return changed;
        };

    // Set window properties
    ImGui::SetNextWindowSize(ImVec2(600, 400));
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::Begin("Monolith.wtf", nullptr, window_flags);

    // Title bar with KEYBINDS button
    ImGui::Text("Monolith.wtf");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    if (ImGui::Button("KEYBINDS →")) {
        // Handle keybinds button click
    }
    ImGui::Separator();

    // Left column
    ImGui::BeginChild("LeftColumn", ImVec2(ImGui::GetWindowWidth() * 0.5f, -1), true);

    // Weapon Section
    ImGui::Text("WEAPON");
    static int weaponIndex = 0;
    const char* weaponItems[] = { "None", "Ak-47", "Thompson", "Lr-300", "SKS", "Custom-SMG", "Python", "SAP", "M249", "HMLMG", "MP5"};
    StyledCombo("##Weapon", &weaponIndex, weaponItems, IM_ARRAYSIZE(weaponItems));

    // Scope Section
    ImGui::Text("Sights");
    static int scopeIndex = 0;
    const char* scopeItems[] = { "ironsights","Holosight", "4x Sight", "8x Sight"};
    StyledCombo("##Scope", &scopeIndex, scopeItems, IM_ARRAYSIZE(scopeItems));

    // Barrel Section
    ImGui::Text("BARREL ATTACHMENT");
    static int barrelIndex = 0;
    const char* barrelItems[] = {"None","Silencer","Muzzle Boost","Muzzle Break"};
    StyledCombo("##Barrel", &barrelIndex, barrelItems, IM_ARRAYSIZE(barrelItems));

    

    // Stats Section
    ImGui::Text("SENSITIVITY   ADS         FOV");
    static float sensitivity = 0.010f;
    static float ads = 0.000f;
    static float fov = 75.0f;

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
    if (ImGui::InputFloat("##Sensitivity", &sensitivity, 0.0f, 0.0f, "%.3f")) {
        recoilSystem.setSensitivity(sensitivity);
    }
    ImGui::SameLine();
    if (ImGui::InputFloat("##ADS", &ads, 0.0f, 0.0f, "%.3f")) {
        recoilSystem.setADS(ads);
    }
    ImGui::SameLine();
    ImGui::InputFloat("##FOV", &fov, 0.0f, 0.0f, "%.0f");
    ImGui::PopItemWidth();

    ImGui::EndChild();

    ImGui::SameLine();

    // Right column
    ImGui::BeginChild("RightColumn", ImVec2(0, -1), true);

    // Smoothness slider
    ImGui::Text("SMOOTHNESS");
    static float smoothness = 0.0f;
    if (StyledSlider("##Smoothness", &smoothness, 0.0f, 100.0f, "%.0f")) {
        recoilSystem.setSmoothness(smoothness);
    }
    ImGui::SameLine();
    ImGui::Text("∿");

    // Randomization sliders
    ImGui::Text("RANDOMIZATION");
    static float randX = 0.0f;
    static float randY = 0.0f;
    if (StyledSlider("##RandX", &randX, 0.0f, 100.0f, "%.0f")) {
        recoilSystem.setRandomizationX(randX);
    }
    ImGui::SameLine();
    ImGui::Text("X");
    if (StyledSlider("##RandY", &randY, 0.0f, 100.0f, "%.0f")) {
        recoilSystem.setRandomizationY(randY);
    }
    ImGui::SameLine();
    ImGui::Text("Y");

    // Recoil Control sliders
    ImGui::Text("RECOIL CONTROL AMOUNT");
    static float recoilX = 100.0f;
    static float recoilY = 100.0f;
    if (StyledSlider("##RecoilX", &recoilX, 0.0f, 100.0f, "%.0f")) {
        recoilSystem.setRecoilControlX(recoilX);
    }
    ImGui::SameLine();
    ImGui::Text("X");
    if (StyledSlider("##RecoilY", &recoilY, 0.0f, 100.0f, "%.0f")) {
        recoilSystem.setRecoilControlY(recoilY);
    }
    ImGui::SameLine();
    ImGui::Text("Y");

    ImGui::EndChild();

    // Bottom checkboxes
    static bool oldRecoil = false;
    static bool attachmentsNoneOnChange = false;
    static bool hideTaskOnTaskbar = false;
    static bool geforceNow = false;
    static bool extendedMagazine = false;
    static bool burstModule = false;
    static bool controlHipfire = false;
    static bool autoFire = false;
    static bool antiAfk = false;
    static bool autoScopeDetection = false;
    static bool autoWeaponDetection = false;

    // Update recoil system when checkboxes change
    if (ImGui::Checkbox("old recoil", &oldRecoil)) recoilSystem.setOldRecoil(oldRecoil);
    if (ImGui::Checkbox("control hipfire", &controlHipfire)) recoilSystem.setControlHipfire(controlHipfire);

    // Calculate positions
    float windowWidth = ImGui::GetWindowWidth();
    float checkboxWidth = windowWidth * 0.45f;
    float startX = 10.0f;
    float middleX = windowWidth * 0.5f;

    // Left column checkboxes
    ImGui::SetCursorPosX(startX);
    ImGui::BeginGroup();
    ImGui::Checkbox("attachments none on change", &attachmentsNoneOnChange);
    ImGui::Checkbox("hide task on taskbar", &hideTaskOnTaskbar);
    ImGui::Checkbox("geforce now", &geforceNow);
    ImGui::Checkbox("extended magazine", &extendedMagazine);
    ImGui::Checkbox("burst module", &burstModule);
    ImGui::EndGroup();

    // Right column checkboxes
    ImGui::SameLine(middleX);
    ImGui::BeginGroup();
    ImGui::Checkbox("auto fire", &autoFire);
    ImGui::Checkbox("anti afk", &antiAfk);
    ImGui::Checkbox("auto scope detection", &autoScopeDetection);
    ImGui::Checkbox("auto weapon detection", &autoWeaponDetection);
    ImGui::EndGroup();


    ImGui::End();
}

int main(int argc, char** argv) {
    // Create recoil system
    RecoilSystem recoilSystem;

    // Set error callback
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(600, 400, "Monolith.wtf", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        fprintf(stderr, "Failed to initialize ImGui GLFW implementation\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        fprintf(stderr, "Failed to initialize ImGui OpenGL3 implementation\n");
        ImGui_ImplGlfw_Shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    CustomStyle();

    // Variables for recoil calculation
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    float recoilX = 0.0f;
    float recoilY = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Calculate recoil
        recoilSystem.calculateRecoil(deltaTime, &recoilX, &recoilY);

        // Apply recoil (you would typically use these values to adjust mouse movement)
        // printf("Recoil: X=%.3f, Y=%.3f\n", recoilX, recoilY);

        // Render GUI
        renderGui(recoilSystem);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}