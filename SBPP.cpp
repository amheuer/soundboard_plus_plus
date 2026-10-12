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
  sf::Sound sound(buffer);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Text("Hello, world %d", 123);
    if (ImGui::Button("Save"))
      MySaveFunction();
    ImGui::InputText("string", buf, IM_COUNTOF(buf));
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    if (!loaded)
      ImGui::Text("Could not load sound.wav");
    else if (ImGui::Button("Play"))
      sound.play();

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
