#include "dibuligs/views/main_view.hpp"


MainView::MainView(std::shared_ptr<ApplicationContext> appctx, std::shared_ptr<UIContext> uictx) : appctx(appctx),
                                                                                                   uictx(uictx)
{}

MainView::~MainView() {}

void MainView::pre_view() {}

void MainView::view()
{
  bool show = true;

  while (!glfwWindowShouldClose(uictx->window))
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    this->processInput();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    // Create the docking environment
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                   ImGuiWindowFlags_NoBackground;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    // 2. Show a simple window.
    {
      ImGui::Begin("Mocap", nullptr, windowFlags);
      ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None);
      if (ImGui::BeginTabItem("Dashboard")) {
        ImGui::Text("Dashboard");
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Tab 2")) {
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
      ImGui::End();

      // Rendering
      ImGui::Render();
      
      int display_w, display_h;
      glfwGetFramebufferSize(uictx->window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      // Update and Render additional Platform Windows
      // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
      //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
          GLFWwindow* backup_current_context = glfwGetCurrentContext();
          ImGui::UpdatePlatformWindows();
          ImGui::RenderPlatformWindowsDefault();
          glfwMakeContextCurrent(backup_current_context);
      }

      glfwSwapBuffers(uictx->window);
    }
  }
}

void MainView::processInput()
{
  if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
  {
    glfwSetWindowShouldClose(uictx->window, true);
  }
}

void MainView::after_first_view() {}

void MainView::post_view()
{
  this->uictx->post_run();
}

std::string MainView::get_name()
{
  return "Main";
}