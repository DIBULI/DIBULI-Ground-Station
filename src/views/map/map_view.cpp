#include "dibuligs/views/map/map_view.hpp"

MapView::MapView(std::shared_ptr<ApplicationContext> appctx,
  std::shared_ptr<UIContext> uictx) : appctx(appctx), uictx(uictx)
{
  this->spaceView = std::make_unique<SpaceView>();
}

MapView::~MapView() {}

void MapView::pre_view() {
  spaceView->pre_view();
}

void MapView::view() {
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
  spaceView->assignPointsPositions({Eigen::Vector3d(0, 0, 0.2), Eigen::Vector3d(5, 0, -1), });
  spaceView->view();

  if (spaceView->chosenPointId != -1) {
    ImGui::Text("Chosen point id: %d", spaceView->chosenPointId);
  }
  
  ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
  ImGui::SliderFloat("Point Size: ", &spaceView->glCameraSphereRadius, 0.01, 0.5);
  
  if (ImGui::Button("Load pointcloud file")) {
    IGFD::FileDialogConfig config;
	  config.path = ".";
    ImGui::SetNextWindowSize(ImVec2(800, 600));
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ply", config);
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      
      // load the point cloud file
      loadPLYFile(filePathName, filePath);
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void MapView::after_first_view() {}

void MapView::post_view() {
  spaceView->post_view();
}

std::string MapView::get_name() {
  return "MapsView";
}

void MapView::loadPLYFile(std::string filePathName, std::string filePath) {
  std::vector<std::tuple<float, float, float>> points;
  std::string filename = filePathName;
  
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  std::string line;
  bool headerEnded = false;

  while (std::getline(file, line)) {
    if (line == "end_header") {
      headerEnded = true;
      break;
    }
  }

  if (!headerEnded) {
    std::cerr << "Invalid PLY file: header not found." << std::endl;
    return;
  }

  while (std::getline(file, line)) {
    std::istringstream ss(line);
    float x, y, z;
    float normalX, normalY, normalZ;
    ss >> x >> y >> z;
    points.emplace_back(x, y, z);
  }

  file.close();
}