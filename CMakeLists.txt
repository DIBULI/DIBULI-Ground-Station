cmake_minimum_required(VERSION 3.2)

project(as-mocap)

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/glfw/install")

find_package(glfw3 REQUIRED)

include_directories(
  ${EIGEN3_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

add_subdirectory(third_party)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/third_party/glm/install")

find_package(glm CONFIG REQUIRED PATHS "${GLM_DIR}")

include_directories(
  include
  ${libusb_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  
  third_party/imgui
  third_party/imgui/backends
)

FILE(GLOB_RECURSE PROJECT_SOURCE_FILES 
  src/contexts/**.cpp
  src/views/**.cpp
  src/main.cpp
)

find_package(OpenGL REQUIRED)
add_executable(dibuli src/main.cpp ${PROJECT_SOURCE_FILES})
target_link_libraries(dibuli ${libusb_LIBRARIES} imgui imgui_backend_glfw imgui_backend_opengl3 glfw glm::glm ${OPENGL_LIBRARIES})

if (WIN32)
  add_custom_command(TARGET dibuli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:glfw>" ${PROJECT_BINARY_DIR}/.
  )
  add_custom_command(TARGET dibuli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:imgui_backend_opengl3>" ${PROJECT_BINARY_DIR}/.
  )
  add_custom_command(TARGET dibuli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:imgui>" ${PROJECT_BINARY_DIR}/.
  )
  add_custom_command(TARGET dibuli
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:imgui_backend_glfw>" ${PROJECT_BINARY_DIR}/.
  )

endif()