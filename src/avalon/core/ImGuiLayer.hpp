#pragma once

#include "Core.hpp"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiLayer {
public:

    void onAttach(GLFWwindow *glfWwindow) {

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(glfWwindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }

    void onUpdate(float deltaTime) {
        // (Your code calls glfwPollEvents())
        // ...
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::ShowDemoWindow(); // Show demo window! :)

        ImGui::Begin("Debug Window");

        ImGuiIO &io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Screen: (%1.f, %1.f)", InputListeners::getInstance().getX(), InputListeners::getInstance().getY());

    }

    void onImGuiRender() {
        ImGui::End();
        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // (Your code calls glfwSwapBuffers() etc.)
    }

    void onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};
