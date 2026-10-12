#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <SFML/Audio.hpp>
#include <iostream>

void MySaveFunction() {
  std::cout << "Saved";
}

int main() {
  // Create window with OpenGL context
  if (!glfwInit()) return 1;
  GLFWwindow* window = glfwCreateWindow(1280, 720, "SoundBoard++", nullptr, nullptr);
  if (!window) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Style changes
  ImGui::StyleColorsClassic();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding    = 6.0f;
  style.ChildRounding     = 6.0f;
  style.PopupRounding     = 6.0f;
  style.FrameRounding     = 6.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding      = 6.0f;
  style.TabRounding       = 6.0f;
  style.MenuItemRounding  = 6.0f;
  style.ImageRounding     = 6.0f;
  style.FontSizeBase      = 18.0f;

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  char buf[256] = "";
  float f = 0.5f;

  sf::SoundBuffer buffer;
  bool loaded = buffer.loadFromFile("laughtrack.mp3");
  sf::Sound laugh_track(buffer);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("SoundBoard++", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (loaded && ImGui::Button("Laugh Track"))
      laugh_track.play();

    ImGui::End();

    // ShowStyleEditor() // Allows customization of the window style
    // ImGui::Begin("Style Editor");
    // ImGui::ShowStyleEditor();
    // ImGui::End();

    // Rendering
    ImGui::Render();
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
