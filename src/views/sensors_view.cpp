#include "dibuligs/views/sensors_view.hpp"

SensorView::SensorView(
  std::shared_ptr<ApplicationContext> appctx,
  std::shared_ptr<UIContext> uictx) : appctx(appctx), uictx(uictx)
{}

SensorView::~SensorView() {
  lasttimeRequestImu = std::chrono::high_resolution_clock::now();
}

void SensorView::pre_view() {}

void SensorView::view() {
  if (!appctx->messageManager->connectionConnected) {
    ImGui::Text("Not connected.");
    return;
  }

  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = now - lasttimeRequestImu;
  std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  if (duration_ms.count() > 20) {
    appctx->messageManager->requestIMUData();
    lasttimeRequestImu = now;
    if (appctx->messageManager->getNewIMUData) {
      appctx->messageManager->readIMUData(this->imuReadings);
      storeImuData(this->imuReadings);
    }
  }
  uint32_t dataOffset = 0;
  if (imuDataIndex.size() > 200) {
    dataOffset = imuDataIndex.size() - 200;
  }

  
  if (ImPlot::BeginPlot("Accelemeter")) {
    static ImPlotAxisFlags xflags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;
    static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;;
    ImPlot::SetupAxes("index", "acc", xflags, yflags);
    ImVec4 lineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("X acc", imuDataIndex.data() + dataOffset, imuAccXBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    lineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("Y acc", imuDataIndex.data() + dataOffset, imuAccYBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    lineColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("Z acc", imuDataIndex.data() + dataOffset, imuAccZBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    ImPlot::EndPlot();
  }
  if (ImPlot::BeginPlot("Gyroscope")) {
      static ImPlotAxisFlags xflags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;
    static ImPlotAxisFlags yflags = ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_RangeFit;;
    ImPlot::SetupAxes("index", "gyro", xflags, yflags);
    ImVec4 lineColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("X gyro", imuDataIndex.data() + dataOffset, imuGyroXBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    lineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("Y gyro", imuDataIndex.data() + dataOffset, imuGyroYBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    lineColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    ImPlot::SetNextLineStyle(lineColor);
    ImPlot::PlotLine("Z gyro", imuDataIndex.data() + dataOffset, imuGyroZBuffer.data() + dataOffset, imuDataIndex.size() > 200 ? 200 : imuDataIndex.size());
    ImPlot::EndPlot();
  }
}

void SensorView::after_first_view() {}

void SensorView::post_view() {}

std::string SensorView::get_name() {
  return "SensorView";
}

void SensorView::storeImuData(float *data) {
  imuDataIndex.push_back(imuDataIndex.size() + 1.0);
  imuAccXBuffer.push_back(data[0]);
  imuAccYBuffer.push_back(data[1]);
  imuAccZBuffer.push_back(data[2]);
  imuGyroXBuffer.push_back(data[3]);
  imuGyroYBuffer.push_back(data[4]);
  imuGyroZBuffer.push_back(data[5]);
}

void SensorView::clearBuffer() {
  imuDataIndex.clear();
  imuAccXBuffer.clear();
  imuAccYBuffer.clear();
  imuAccZBuffer.clear();
  imuGyroXBuffer.clear();
  imuGyroYBuffer.clear();
  imuGyroZBuffer.clear();
}

