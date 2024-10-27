#ifndef MAP_VIEW_HPP
#define MAP_VIEW_HPP

#include <chrono>
#include <iomanip>
#include <ctime>

#include "imgui_internal.h"
#include "ImGuiFileDialog.h"
#include "dibuligs/views/iview.hpp"

#include "dibuligs/contexts/application_context.hpp"
#include "dibuligs/contexts/ui_context.hpp"

#include "dibuligs/views/map/space_view.hpp"

class MapView : public IView{
public:
  MapView(std::shared_ptr<ApplicationContext> appctx,
           std::shared_ptr<UIContext> uictx);
  ~MapView();

  void pre_view() override;
  void view() override;
  void after_first_view() override;
  void post_view() override;
  std::string get_name() override;

  void loadPLYFile(std::string filePathName, std::string filePath);

  std::shared_ptr<ApplicationContext> appctx;
  std::shared_ptr<UIContext> uictx;

  std::unique_ptr<SpaceView> spaceView;
};

#endif /* MAP_VIEW_HPP */
