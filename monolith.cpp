#include "monolith.h"
#include <chrono>
#include <thread>

void CustomStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(10, 8);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
}

int main(int argc, char** argv) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(800, 600, "Monolith", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    CustomStyle();

    RecoilSystem recoilSystem;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderGui(recoilSystem);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
