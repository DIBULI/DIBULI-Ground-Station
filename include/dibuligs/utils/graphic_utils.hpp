#ifndef GRAPHIC_UTILS_HPP
#define GRAPHIC_UTILS_HPP

#include <iostream>
#include <memory>

#include <GLFW/glfw3.h>
#include <Eigen/Core>

class GraphicUtils {
public:
  GraphicUtils();
  ~GraphicUtils();

  /**
   * @brief  
   * @note   
   * @param  &texture: 
   * @param  width: 
   * @param  height: 
   * @param  image: 3 channels image
   * @retval None
   */
  static void getCameraVertexData(Eigen::Matrix3d rot, Eigen::Vector3d pos, std::vector<float> &vertices);

  static void getSphereVerticeData(Eigen::Vector3d pos, float radius, unsigned int sectorCount, unsigned int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices);

  static std::shared_ptr<GraphicUtils> instance();

  static std::shared_ptr<GraphicUtils> graphicUtils;
};

#endif /* GRAPHIC_UTILS_HPP */
