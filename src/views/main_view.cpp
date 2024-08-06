#include "dibuligs/views/main_view.hpp"


MainView::MainView(std::shared_ptr<ApplicationContext> appctx, std::shared_ptr<UIContext> uictx) 
: appctx(appctx),
uictx(uictx), sensorView(appctx, uictx)
{
  
}

MainView::~MainView() {}

void MainView::pre_view() {
  sensorView.pre_view();
}

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
      ImGui::Begin("Ground Station", nullptr, windowFlags);
      
      ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None);
      if (ImGui::BeginTabItem("Dashboard")) {
        ImGui::Text("Dashboard");
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Sensors")) {
        if (!appctx->messageManager->requestIMUReading) {
          sensorView.clearBuffer();
          std::cout << "Start request imu data." << std::endl;
          appctx->messageManager->requestIMUReading = true;
        }
        sensorView.view();
        ImGui::EndTabItem();
      } else if (appctx->messageManager->requestIMUReading) {
        std::cout << "Stop request imu data." << std::endl;
        appctx->messageManager->requestIMUReading = false;
      }
      ImGui::EndTabBar();

      ImGui::End();

      // Create status bar
      ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.95));
      ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y * 0.05));
      ImGui::Begin("Status Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

      float windowWidth = ImGui::GetContentRegionAvail().x;

      ImGui::SetCursorPosX(windowWidth - 480);

      ImGui::Text("Available Serial Ports:");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(200.0f);
      if (ImGui::BeginCombo("##listbox", appctx->messageManager->serials[currentPortIndex].portName)) {
        for (int i = 0; i < appctx->messageManager->serials.size(); i++) {
          bool isSelected = (currentPortIndex == i);
          if (ImGui::Selectable( appctx->messageManager->serials[i].portName, isSelected)) {
            currentPortIndex = i;
          }
          if (isSelected) {
              ImGui::SetItemDefaultFocus();
          }
        }
        
        
        ImGui::EndCombo();
      }

      ImGui::SameLine();
      ImGui::SetNextItemWidth(70.0f);
      if (appctx->messageManager->connectionConnected) {
        if (ImGui::Button("Disconnect##port", ImVec2(100, 0))) {
          appctx->messageManager->disconnect();
        }
      } else {
        if (ImGui::Button("Connect##port", ImVec2(100, 0))) {
          appctx->messageManager->connect(0, currentPortIndex);
        }
      }

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
  sensorView.post_view();
}

std::string MainView::get_name()
{
  return "Main";
}