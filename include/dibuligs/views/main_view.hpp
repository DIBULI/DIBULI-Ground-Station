#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include "imgui_internal.h"
#include <memory>

#include "dibuligs/views/iview.hpp"
#include "dibuligs/contexts/application_context.hpp"
#include "dibuligs/contexts/ui_context.hpp"

#include "dibuligs/views/sensors_view.hpp"

class MainView: public IView{
public:
  MainView(std::shared_ptr<ApplicationContext> appctx,
           std::shared_ptr<UIContext> uictx);
  ~MainView();

  void pre_view() override;
  void view() override;
  void after_first_view() override;
  void post_view() override;
  std::string get_name() override;

  void processInput();

  std::shared_ptr<ApplicationContext> appctx;
  std::shared_ptr<UIContext> uictx;

  //views
  SensorView sensorView;

  int currentPortIndex = 0;
};

#endif /* MAIN_VIEW_HPP */
