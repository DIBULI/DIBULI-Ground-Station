#ifndef SENSORS_VIEW_HPP
#define SENSORS_VIEW_HPP

#include <chrono>
#include <iomanip>
#include <ctime>

#include "imgui_internal.h"
#include "dibuligs/views/iview.hpp"

#include "dibuligs/contexts/application_context.hpp"
#include "dibuligs/contexts/ui_context.hpp"

class SensorView: public IView{
public:
  SensorView(std::shared_ptr<ApplicationContext> appctx,
           std::shared_ptr<UIContext> uictx);
  ~SensorView();

  void pre_view() override;
  void view() override;
  void after_first_view() override;
  void post_view() override;
  std::string get_name() override;

  std::shared_ptr<ApplicationContext> appctx;
  std::shared_ptr<UIContext> uictx;

  std::chrono::high_resolution_clock::time_point lasttimeRequestImu;

  float imuReadings[7];
  std::vector<float> imuDataIndex;
  std::vector<float> imuAccXBuffer;
  std::vector<float> imuAccYBuffer;
  std::vector<float> imuAccZBuffer;

  std::vector<float> imuGyroXBuffer;
  std::vector<float> imuGyroYBuffer;
  std::vector<float> imuGyroZBuffer;

  void storeImuData(float *data);
  void clearBuffer();
};

#endif /* SENSORS_VIEW_HPP */
