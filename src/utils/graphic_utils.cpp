#include "dibuligs/utils/graphic_utils.hpp"

std::shared_ptr<GraphicUtils> GraphicUtils::graphicUtils = nullptr;

GraphicUtils::GraphicUtils() {}

GraphicUtils::~GraphicUtils() {}

std::shared_ptr<GraphicUtils> GraphicUtils::instance() {
  if (!GraphicUtils::graphicUtils) {
    GraphicUtils::graphicUtils = std::make_shared<GraphicUtils>();
  }
  return GraphicUtils::graphicUtils;
}

void GraphicUtils::getCameraVertexData(Eigen::Matrix3d rot, Eigen::Vector3d pos, std::vector<float> &vertices) {
  Eigen::Vector3d pos1;
  Eigen::Vector3d pos2;
  Eigen::Vector3d pos3;
  Eigen::Vector3d pos4;

  pos1 << 0.2, 0.1, 0.1;
  pos2 << -0.2, 0.1, 0.1;
  pos3 << -0.2, -0.1, 0.1;
  pos4 << 0.2, -0.1, 0.1;

  pos1 = rot * pos1 + pos;
  pos2 = rot * pos2 + pos;
  pos3 = rot * pos3 + pos;
  pos4 = rot * pos4 + pos;

  vertices.push_back(pos.x());
  vertices.push_back(pos.y());
  vertices.push_back(pos.z());
  vertices.push_back(pos1.x());
  vertices.push_back(pos1.y());
  vertices.push_back(pos1.z());

  vertices.push_back(pos.x());
  vertices.push_back(pos.y());
  vertices.push_back(pos.z());
  vertices.push_back(pos2.x());
  vertices.push_back(pos2.y());
  vertices.push_back(pos2.z());

  vertices.push_back(pos.x());
  vertices.push_back(pos.y());
  vertices.push_back(pos.z());
  vertices.push_back(pos3.x());
  vertices.push_back(pos3.y());
  vertices.push_back(pos3.z());

  vertices.push_back(pos.x());
  vertices.push_back(pos.y());
  vertices.push_back(pos.z());
  vertices.push_back(pos4.x());
  vertices.push_back(pos4.y());
  vertices.push_back(pos4.z());

  vertices.push_back(pos1.x());
  vertices.push_back(pos1.y());
  vertices.push_back(pos1.z());
  vertices.push_back(pos2.x());
  vertices.push_back(pos2.y());
  vertices.push_back(pos2.z());

  vertices.push_back(pos2.x());
  vertices.push_back(pos2.y());
  vertices.push_back(pos2.z());
  vertices.push_back(pos3.x());
  vertices.push_back(pos3.y());
  vertices.push_back(pos3.z());

  vertices.push_back(pos3.x());
  vertices.push_back(pos3.y());
  vertices.push_back(pos3.z());
  vertices.push_back(pos4.x());
  vertices.push_back(pos4.y());
  vertices.push_back(pos4.z());

  vertices.push_back(pos4.x());
  vertices.push_back(pos4.y());
  vertices.push_back(pos4.z());
  vertices.push_back(pos1.x());
  vertices.push_back(pos1.y());
  vertices.push_back(pos1.z());
}

void GraphicUtils::getSphereVerticeData(Eigen::Vector3d pos, float radius, unsigned int sectorCount, unsigned int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
  const float PI = 3.14159265359;
  for (unsigned int y = 0; y <= sectorCount; ++y){
    for (unsigned int x = 0; x <= sectorCount; ++x)
    {
      float xSegment = (float)x / (float)sectorCount;
      float ySegment = (float)y / (float)sectorCount;
      float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius + pos.x();
      float yPos = std::cos(ySegment * PI) * radius + pos.y();
      float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * radius + pos.z();

      vertices.push_back(xPos);
      vertices.push_back(yPos);
      vertices.push_back(zPos);
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
    }
  }

  bool oddRow = false;
  for (unsigned int y = 0; y < sectorCount; ++y) {
    if (!oddRow) // even rows: y == 0, y == 2; and so on
    {
      for (unsigned int x = 0; x <= sectorCount; ++x) {
        indices.push_back(y       * (sectorCount + 1) + x);
        indices.push_back((y + 1) * (sectorCount + 1) + x);
      }
    }
    else {
      for (int x = sectorCount; x >= 0; --x) {
        indices.push_back((y + 1) * (sectorCount + 1) + x);
        indices.push_back(y       * (sectorCount + 1) + x);
      }
    }
    oddRow = !oddRow;
  }
}