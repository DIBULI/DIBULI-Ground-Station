cmake_minimum_required(VERSION 3.2)

project(as-mocap)

enable_testing()

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/glfw/install")

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(eigen3 REQUIRED)

include_directories(
  ${Vulkan_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

add_subdirectory(third_party)
add_subdirectory(libs)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/third_party/glm/install")

find_package(glm CONFIG REQUIRED PATHS "${GLM_DIR}")
find_package(glfw3 REQUIRED)

FILE(GLOB_RECURSE PROJECT_SOURCE_FILES 
  src/contexts/**.cpp
  src/views/**.cpp
  src/structs/**.cpp
  src/utils/**.cpp
  src/managers/**.cpp
)

include_directories(
  include
  ${libusb_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
  ${CSerialPort_INCLUDE_DIR}
  libs/DProtocol/include
  third_party/imgui
  third_party/imgui/backends
)

find_package(OpenGL REQUIRED)

add_executable(dibuligs src/main.cpp ${PROJECT_SOURCE_FILES})
target_link_libraries(dibuligs ${libusb_LIBRARIES} imgui imgui_backend_glfw imgui_backend_opengl3
glfw glm::glm ${OPENGL_LIBRARIES} ${CSerialPort_LIBRARY} dprotocol)

if (APPLE)
  target_link_libraries(dibuligs "-framework Foundation" "-framework IOKit")
endif()

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