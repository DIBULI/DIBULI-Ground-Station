#ifndef SPACE_VIEW_HPP
#define SPACE_VIEW_HPP

#include <map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Eigen/Core>

#include "dibuligs/utils/graphic_utils.hpp"
#include "dibuligs/views/iview.hpp"
#include "dibuligs/contexts/application_context.hpp"

class SpaceView: public IView {
public:
  SpaceView();
  ~SpaceView();

  void pre_view() override;
  void view() override;
  void after_first_view() override;
  void post_view() override;
  std::string get_name() override;

  void assignPointsPositions(std::vector<Eigen::Vector3d> points);

  std::mutex cameraDataUpdateMutex;

  GLuint fb, textureColorbuffer, rbo, shaderProgram, VAO, VBO;

  float longitude = 0.0f;
  float latitude = 0.0f;
  float originalLongitude = 0.0f;
  float originalLatitude = 0.0f;

  std::vector<GLuint> pointsVAO, pointsVBO, pointsEBO;
  std::vector<std::vector<float>> pointVertices;
  std::vector<std::vector<unsigned int>> pointIndices;
  std::vector<Eigen::Vector3d> points;

  glm::mat4 projection;
  glm::mat4 viewMat;
  glm::mat4 model;
  glm::mat4 rotationMatrix;
  glm::vec3 cameraPosition;
  glm::vec3 originalCameraPosition;
  glm::vec3 camera;
  glm::vec3 cameraCenter;
  glm::vec3 originalCameraCenter;
  glm::vec3 upAxis;
  glm::vec3 originalUpAxis;

  float glCameraSphereRadius = 0.01f;

  float cameraViewSphereRadius = 10.0f;

  bool isRightMouseDown = false;
  float mousePositionXRightMouse;
  float mousePositionYRightMouse;

  bool isLeftMouseDown = false;
  float mousePositionXLeftMouse;
  float mousePositionYLeftMouse;
};


#endif /* SPACE_VIEW_HPP */
