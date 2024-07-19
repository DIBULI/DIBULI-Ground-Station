cmake_minimum_required(VERSION 3.2)

project(as-mocap)

enable_testing()

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

FILE(GLOB_RECURSE PROJECT_SOURCE_FILES 
  src/contexts/**.cpp
  src/views/**.cpp
  src/structs/**.cpp
)

add_library(dibuli_lib ${PROJECT_SOURCE_FILES})

target_include_directories(dibuli_lib PUBLIC
  include
  ${libusb_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  
  third_party/imgui
  third_party/imgui/backends
)

find_package(OpenGL REQUIRED)
target_link_libraries(dibuli_lib ${libusb_LIBRARIES} imgui imgui_backend_glfw imgui_backend_opengl3 glfw glm::glm ${OPENGL_LIBRARIES})

add_executable(dibuli src/main.cpp)
target_link_libraries(dibuli dibuli_lib)

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


add_executable(circular_byte_array_test tests/test_circular_byte_array.cpp)
target_link_libraries(circular_byte_array_test dibuli_lib)
add_test(NAME TestCircularByteArray
         COMMAND circular_byte_array_test)