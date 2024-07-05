cmake_minimum_required(VERSION 3.13)  # CMake version check

# add all the third party libraries CMakeConfig detectable
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/eigen/install")
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/glfw/install")
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/glm/install")
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/third_party/eigen/install")

# set libusb
SET(libusb_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/third_party/libusb/include")
SET(libusb_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/third_party/libusb/include" PARENT_SCOPE)

if (WIN32)
    add_library(libusb STATIC IMPORTED GLOBAL)
    set_property(TARGET libusb PROPERTY
                IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/third_party/libusb/libs/win32/libusb-1.0.a")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(libusb REQUIRED libusb-1.0)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(imgui)
include_directories(imgui/backends)

SET(IMGUI_PATH imgui)

# You may have to manually set CMAKE_C_COMPILER and CMAKE_CXX_COMPILER respectively to clang
# set(CMAKE_C_COMPILER "/usr/local/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")

# Add all .cpp files of project root directory as source file
set(IMGUI_SOURCE_FILES 
${IMGUI_PATH}/imgui.cpp 
${IMGUI_PATH}/imgui_draw.cpp
${IMGUI_PATH}/imgui_demo.cpp 
${IMGUI_PATH}/imgui_tables.cpp 
${IMGUI_PATH}/imgui_widgets.cpp) # Add all .cpp files of project root directory as source files


add_library(imgui ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC .)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_BACKEND_GLFW "Build backend GLFW" ON)

message(STATUS ${GLFW_LIBRARIES})

if(${BUILD_BACKEND_GLFW})
    set(BACKEND_FILE ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp)
    add_library(imgui_backend_glfw ${BACKEND_FILE})
    if (WIN32 OR APPLE)
        add_dependencies(imgui_backend_glfw glfw imgui)
        target_link_libraries(imgui_backend_glfw imgui glfw)
    endif()
endif()

set(BACKEND_FILE ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
add_library(imgui_backend_opengl3 ${BACKEND_FILE})
target_link_libraries(imgui_backend_opengl3 imgui)
set(EXAMPLE_MAIN_FILE ${IMGUI_PATH}/examples/example_glfw_opengl3/main.cpp)
add_executable(example_glfw_opengl3 ${EXAMPLE_MAIN_FILE})
target_link_libraries(example_glfw_opengl3 imgui imgui_backend_glfw imgui_backend_opengl3 glfw ${OPENGL_LIBRARIES})


if (WIN32)
    add_custom_command(TARGET example_glfw_opengl3
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:glfw>" ${PROJECT_BINARY_DIR}/third_party/.
    )
endif()