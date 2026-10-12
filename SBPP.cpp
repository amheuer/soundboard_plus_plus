#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <SFML/Audio.hpp>
#include <portable-file-dialogs.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

void MySaveFunction() {
  std::cout << "Saved";
}

struct SoundButton {
  std::string name, file_name;
  sf::SoundBuffer buffer;
  sf::Sound sound{buffer};
};

// file_name is relative to sounds/
bool addSound(std::list<SoundButton>& sounds, const std::string& name, const std::string& file_name) {
  SoundButton& s = sounds.emplace_back();
  s.name = name;
  s.file_name = file_name;
  if (s.buffer.loadFromFile("sounds/" + file_name)) return true;
  std::cerr << "Could not load sounds/" << file_name << '\n';
  sounds.pop_back();
  return false;
}

void saveSounds(const std::list<SoundButton>& sounds) {
  std::ofstream csv("sounds.csv");
  for (auto& s : sounds) csv << s.name << ',' << s.file_name << '\n';
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
  style.CellPadding = {6.0f, 6.0f};

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  char buf[256] = "";
  float f = 0.5f;

  // Each line of sounds.csv: button name,file name
  std::list<SoundButton> sounds; // Used instead of vector or deque because it cleanly maintains buffers when new elements are added
  std::ifstream csv("sounds.csv");
  std::string button_name, file_name;
  while (std::getline(csv, button_name, ',') && std::getline(csv, file_name))
    addSound(sounds, button_name, file_name);


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

    // Add sounds: pick files, copy them into sounds/, name the buttons after the files
    if (ImGui::Button("Add sound...")) {
      auto files = pfd::open_file("Add sound", "", {"Audio files", "*.wav *.mp3 *.ogg *.flac"},
                                  pfd::opt::multiselect).result();  // blocks until the dialog closes
      for (const std::filesystem::path src : files) {
        std::filesystem::path dst = "sounds" / src.filename();
        if (!std::filesystem::exists(dst))
          std::filesystem::copy_file(src, dst);
        if (addSound(sounds, src.stem().string(), src.filename().string()))
          saveSounds(sounds);
      }
    }

    const float tile = 120.0f;  // button height and minimum width, in pixels
    int columns = std::max(1, int(ImGui::GetContentRegionAvail().x / (tile + style.ItemSpacing.x)));

    if (ImGui::BeginTable("grid", columns)) {
      for (auto& s : sounds) {
        ImGui::TableNextColumn();
        if (ImGui::Button(s.name.c_str(), {-FLT_MIN, tile}))  // -FLT_MIN = fill the cell's width
          s.sound.play();
      }
      ImGui::EndTable();
    }

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
